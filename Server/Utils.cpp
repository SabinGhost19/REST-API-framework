#include "Utils.h"
#include <string.h>
#include <iostream>

//-----------------------------------------------------
std::string getStatusMessage(int code)
{
    switch (code)
    {
    case 200:
        return "200 OK";
    case 201:
        return "201 Created";
    case 202:
        return "202 Accepted";
    case 204:
        return "204 No Content";
    case 301:
        return "301 Moved Permanently";
    case 302:
        return "302 Found";
    case 304:
        return "304 Not Modified";
    case 400:
        return "400 Bad Request";
    case 401:
        return "401 Unauthorized";
    case 403:
        return "403 Forbidden";
    case 404:
        return "404 Not Found";
    case 405:
        return "405 Method Not Allowed";
    case 408:
        return "408 Request Timeout";
    case 409:
        return "409 Conflict";
    case 410:
        return "410 Gone";
    case 411:
        return "411 Length Required";
    case 413:
        return "413 Payload Too Large";
    case 415:
        return "415 Unsupported Media Type";
    case 429:
        return "429 Too Many Requests";
    case 500:
        return "500 Internal Server Error";
    case 501:
        return "501 Not Implemented";
    case 502:
        return "502 Bad Gateway";
    case 503:
        return "503 Service Unavailable";
    case 504:
        return "504 Gateway Timeout";
    default:
        return std::to_string(code) + " Unknown Status"; // Cod necunoscut
    }
}

std::string getContentTypeHeader(ContentType type)
{
    switch (type)
    {
    case ContentType::TextPlain:
        return "text/plain";
    case ContentType::TextHtml:
        return "text/html";
    case ContentType::ApplicationJson:
        return "application/json";
    case ContentType::ApplicationXml:
        return "application/xml";
    case ContentType::ImagePng:
        return "image/png";
    case ContentType::ImageJpeg:
        return "image/jpeg";
    case ContentType::VideoMp4:
        return "video/mp4";
    case ContentType::AudioMpeg:
        return "audio/mpeg";
    default:
        return "text/plain"; // Valoare implicita
    }
}

std::string getConnectionHeader(ConnectionType type)
{
    switch (type)
    {
    case ConnectionType::Close:
        return "Connection: close";
    case ConnectionType::KeepAlive:
        return "Connection: keep-alive";
    case ConnectionType::Upgrade:
        return "Connection: upgrade";
    default:
        return "Connection: close"; // Valoare implicita
    }
}
