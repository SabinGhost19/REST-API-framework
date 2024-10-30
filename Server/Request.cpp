#include "Request.h"

std::string Request::GetQueryParam(const std::string &key)
{
    if (this->queryParams.find(key) != this->queryParams.end())
    {
        return this->queryParams[key];
    }
    return "";
}
std::string Request::GetHeader(const std::string &key)
{
    if (headers.find(key) != headers.end())
    {
        return headers[key];
    }
    return "";
}
void Request::ParseQueryParams()
{
    size_t questionMarkPos = path.find("?");
    if (questionMarkPos != std::string::npos)
    {
        std::string queryString = path.substr(questionMarkPos + 1);
        std::stringstream ss(queryString);
        std::string pair;
        while (std::getline(ss, pair, '&'))
        {
            size_t equalPos = pair.find("=");
            if (equalPos != std::string::npos)
            {
                std::string key = pair.substr(0, equalPos);
                std::string value = pair.substr(equalPos + 1);
                queryParams[key] = value;
            }
        }
    }
}