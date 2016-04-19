#include "TextDB.h"


TextDB::TextDB(const std::string &fileName)
    : fileName(fileName)
{
    load();
}

void TextDB::load()
{
    std::ifstream ifs(fileName, std::ios_base::in);
    std::string txt = FileUtils::ReadAllText(ifs);
    Lexer lex(txt);
    auto tokens = lex.Do();
    
    std::vector<std::string> vecbuff;
    std::string strbuff;
    for (Token token : tokens)
    {
        if (token.Type == TOKENTYPE_VALUE)
        {
            strbuff += StringUtils::Unescape(token.Value);
        }
        else if (token.Type == TOKENTYPE_SEPARATOR)
        {
            vecbuff.push_back(strbuff);
            strbuff = "";
        }
        else if (token.Type == TOKENTYPE_NEWLINE)
        {
            if (strbuff != "")
            {
                vecbuff.push_back(strbuff);
                strbuff = "";
            }
            
            if (vecbuff.size() > 0)
            {
                std::vector<std::string> &val = this->operator[](vecbuff[0]);
                val.insert(val.begin(), vecbuff.begin() + 1, vecbuff.end());
                vecbuff.clear();
            }
        }
    }
    
    if (strbuff != "")
    {
        vecbuff.push_back(strbuff);
        strbuff = "";
    }
    
    if (vecbuff.size() > 0)
    {
        std::vector<std::string> &val = this->operator[](vecbuff[0]);
        val.insert(val.begin(), vecbuff.begin() + 1, vecbuff.end());
        vecbuff.clear();
    }
}

void TextDB::Sync()
{
    std::ofstream ofs(fileName + ".txt", std::ios_base::out);
    for (TextDBCollection::iterator iter = begin(); iter != end(); ++iter)
    {
        ofs << StringUtils::Escape(iter->first);
        std::vector<std::string> &val = iter->second;
        if (val.size() > 0)
        {
            ofs << ", ";
            for (std::vector<std::string>::iterator valiter = val.begin(); valiter != val.end(); ++valiter)
            {
                ofs << StringUtils::Escape(*valiter);
                if (valiter != val.end() - 1)
                {
                    ofs << ", ";
                }
            }
        }
        ofs << std::endl;
    }
}

TextDB::~TextDB()
{
    Sync();
}