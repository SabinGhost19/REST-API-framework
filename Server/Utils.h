#ifndef UTILS_H
#define UTILS_H
#include <string>

enum class ContentType
{
    TextPlain,
    TextHtml,
    ApplicationJson,
    ApplicationXml,
    ImagePng,
    ImageJpeg,
    VideoMp4,
    AudioMpeg
};

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

enum class ConnectionType
{
    Close,
    KeepAlive,
    Upgrade // Conexiunea este cu upgrade (de exemplu, WebSocket)
};

std::string getContentTypeHeader(ContentType type);
std::string getConnectionHeader(ConnectionType type);
std::string getStatusMessage(int code);

#endif // UTILS_H