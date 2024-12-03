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
enum class DataBase_Type{
    MySQL,
    Postgres,
};

enum class ConnectionType
{
    Close,
    KeepAlive,
    Upgrade 
};

std::string getContentTypeHeader(ContentType type);
std::string getConnectionHeader(ConnectionType type);
std::string getStatusMessage(int code);

#endif 