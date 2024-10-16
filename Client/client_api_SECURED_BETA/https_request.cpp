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
