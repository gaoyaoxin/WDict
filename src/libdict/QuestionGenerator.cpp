#include "QuestionGenerator.h"

QuestionGenerator::QuestionGenerator(std::default_random_engine &engine, IDictionary &dict, WordIterator &iter, IQuestionBuilder &qb)
    : engine(engine), dict(dict), iter(iter), qb(qb)
{
    handlers = {
        std::bind(&QuestionGenerator::genTF, this, std::placeholders::_1),
        std::bind(&QuestionGenerator::genSelDesc, this, std::placeholders::_1),
        std::bind(&QuestionGenerator::genSelWord, this, std::placeholders::_1),
        std::bind(&QuestionGenerator::genInputWord, this, std::placeholders::_1)
    };
}

void QuestionGenerator::Begin()
{
    
}

bool QuestionGenerator::Generate(std::string &out_answer)
{
    if (iter.HaveNext())
    {
        std::string word = iter.Next();
        wrongAnswerIter = std::make_shared<RandomWordIterator>(engine, dict, [=] (const std::string &w)
        {
            return w != word;
        });
        
        out_answer = handlers[dd(engine)](word);
        return true;
    }
    else
    {
        return false;
    }
}

void QuestionGenerator::End()
{
    
}
    
std::string QuestionGenerator::genTF(const std::string &word)
{
    std::uniform_int_distribution<size_t> answerDist(0, 1);
    bool correctAnswer = answerDist(engine);
    
    qb.BeginQuestion("请判断: \n");
    
    std::string descWord;
    
    if (correctAnswer)
    {
        descWord = word;
    }
    else
    {
        descWord = wrongAnswerIter->Next();
    }
    
    WordInfo wi = dict.GetWord(descWord);
    std::uniform_int_distribution<size_t> dist(0, wi.Desc.size() - 1);
    qb.BeginQuestion(word + "有\"" + wi.Desc[dist(engine)] + "\"的意思。");
    
    qb.BeginOption('T', "正确"); qb.EndOption();
    qb.BeginOption('F', "错误"); qb.EndOption();
    
    qb.EndQuestion();
    
    return correctAnswer ? "T" : "F";
}

std::string QuestionGenerator::genSelDesc(const std::string &word)
{
    const size_t count = 4;
    
    std::uniform_int_distribution<size_t> answerDist(0, count - 1);
    std::size_t correctAnswer = answerDist(engine);
    
    WordInfo wi = dict.GetWord(word);
    std::uniform_int_distribution<size_t> dist(0, wi.Desc.size() - 1);
    std::string desc = wi.Desc[dist(engine)];
    
    qb.BeginQuestion(word + "有如下哪个意思?");
    for (size_t i = 0; i < count; ++i)
    {
        if (i == correctAnswer)
        {
            qb.BeginOption('A' + i, desc); qb.EndOption();
        }
        else
        {
            WordInfo wwi = dict.GetWord(wrongAnswerIter->Next());
            std::uniform_int_distribution<size_t> dist(0, wwi.Desc.size() - 1);
            qb.BeginOption('A' + i, wwi.Desc[dist(engine)]); qb.EndOption();
        }
    }
    qb.EndQuestion();
    
    return std::string(1, 'A' + correctAnswer);
}

std::string QuestionGenerator::genSelWord(const std::string &word)
{
    const size_t count = 4;
    
    std::uniform_int_distribution<size_t> answerDist(0, count - 1);
    size_t correctAnswer = answerDist(engine);
    
    WordInfo wi = dict.GetWord(word);
    std::uniform_int_distribution<size_t> dist(0, wi.Desc.size() - 1);
    
    qb.BeginQuestion("哪一个单词有\"" + wi.Desc[dist(engine)] + "\"的意思?");
    for (size_t i = 0; i < count; ++i)
    {
        if (i == correctAnswer)
        {
            qb.BeginOption('A' + i, word); qb.EndOption();
        }
        else
        {
            qb.BeginOption('A' + i, wrongAnswerIter->Next()); qb.EndOption();
        }
    }
    qb.EndQuestion();
    
    return std::string(1, 'A' + correctAnswer);
}

std::string QuestionGenerator::genInputWord(const std::string &word)
{
    WordInfo wi = dict.GetWord(word);
    std::uniform_int_distribution<size_t> dist(0, wi.Desc.size() - 1);
    
    qb.BeginQuestion("哪一个单词有\"" + wi.Desc[dist(engine)] + "\"的意思?");
    qb.BeginTextInput(); qb.EndTextInput();
    qb.EndQuestion();
    
    return word;
}