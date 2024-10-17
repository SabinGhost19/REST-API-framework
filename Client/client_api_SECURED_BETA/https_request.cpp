#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void initOpenSSL()
{
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

void cleanupOpenSSL()
{
    EVP_cleanup();
}

SSL_CTX *createContext()
{
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    SSL_CTX_set_default_verify_paths(ctx);
    return ctx;
}

void cleanupContext(SSL_CTX *ctx)
{
    SSL_CTX_free(ctx);
}

void printResponse(SSL *ssl)
{
    char buffer[4096];
    int bytesReceived;

    while ((bytesReceived = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesReceived] = '\0'; // Terminare șir
        std::cout << buffer;
    }
}

int main()
{
    // Inițializare OpenSSL
    initOpenSSL();

    // Crearea contextului SSL
    SSL_CTX *ctx = createContext();

    // Crearea socketului
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Socket creation error\n";
        return 1;
    }

    // Obținerea adresei serverului
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // sau AF_INET6 pentru IPv6
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("catfact.ninja", "443", &hints, &res) != 0)
    {
        std::cerr << "getaddrinfo error\n";
        close(sockfd);
        return 1;
    }

    // Conectare la server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0)
    {
        std::cerr << "Connection failed\n";
        freeaddrinfo(res);
        close(sockfd);
        return 1;
    }

    // Curățare resurse
    freeaddrinfo(res);

    // Crearea obiectului SSL
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    // Conectare SSL
    if (SSL_connect(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sockfd);
        cleanupContext(ctx);
        cleanupOpenSSL();
        return 1;
    }

    // Cererea HTTP
    std::string httpRequest = "GET /fact HTTP/1.1\r\n"
                              "Host: catfact.ninja\r\n"
                              "Connection: close\r\n"
                              "\r\n";

    // Trimiterea cererii
    SSL_write(ssl, httpRequest.c_str(), httpRequest.length());

    // Afișarea răspunsului
    printResponse(ssl);

    // Curățare
    SSL_free(ssl);
    close(sockfd);
    cleanupContext(ctx);
    cleanupOpenSSL();

    return 0;
}

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// Not sure what headers are needed or not
// This code (theoretically) writes "Hello World, 123" to a socket over a secure TLS connection
// compiled with g++ -Wall -o client.out client.cpp -L/usr/lib -lssl -lcrypto
// Based off of: https://www.cs.utah.edu/~swalton/listings/articles/ssl_client.c
// Some of the code was taken from this post: https://stackoverflow.com/questions/52727565/client-in-c-use-gethostbyname-or-getaddrinfo

const int ERROR_STATUS = -1;

SSL_CTX *InitSSL_CTX(void)
{
    const SSL_METHOD *method = TLS_client_method(); /* Create new client-method instance */
    SSL_CTX *ctx = SSL_CTX_new(method);

    if (ctx == nullptr)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

int OpenConnection(const char *hostname, const char *port)
{
    struct hostent *host;
    if ((host = gethostbyname(hostname)) == nullptr)
    {
        perror(hostname);
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints = {0}, *addrs;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    const int status = getaddrinfo(hostname, port, &hints, &addrs);
    if (status != 0)
    {
        fprintf(stderr, "%s: %s\n", hostname, gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    int sfd, err;
    for (struct addrinfo *addr = addrs; addr != nullptr; addr = addr->ai_next)
    {
        sfd = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
        if (sfd == ERROR_STATUS)
        {
            err = errno;
            continue;
        }

        if (connect(sfd, addr->ai_addr, addr->ai_addrlen) == 0)
        {
            break;
        }

        err = errno;
        sfd = ERROR_STATUS;
        close(sfd);
    }

    freeaddrinfo(addrs);

    if (sfd == ERROR_STATUS)
    {
        fprintf(stderr, "%s: %s\n", hostname, strerror(err));
        exit(EXIT_FAILURE);
    }
    return sfd;
}

void DisplayCerts(SSL *ssl)
{
    X509 *cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if (cert != nullptr)
    {
        printf("Server certificates:\n");
        char *line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        delete line;
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        delete line;
        X509_free(cert);
    }
    else
    {
        printf("Info: No client certificates configured.\n");
    }
}

int main(int argc, char const *argv[])
{
    SSL_CTX *ctx = InitSSL_CTX();
    SSL *ssl = SSL_new(ctx);
    if (ssl == nullptr)
    {
        fprintf(stderr, "SSL_new() failed\n");
        exit(EXIT_FAILURE);
    }

    // Host is hardcoded to localhost for testing purposes
    const int sfd = OpenConnection("127.0.0.1", argv[1]);
    SSL_set_fd(ssl, sfd);

    const int status = SSL_connect(ssl);
    if (status != 1)
    {
        SSL_get_error(ssl, status);
        ERR_print_errors_fp(stderr); // High probability this doesn't do anything
        fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n", status);
        exit(EXIT_FAILURE);
    }

    printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
    DisplayCerts(ssl);
    const char *chars = "Hello World, 123!";
    SSL_write(ssl, chars, strlen(chars));
    SSL_free(ssl);
    close(sfd);
    SSL_CTX_free(ctx);
    return 0;
}