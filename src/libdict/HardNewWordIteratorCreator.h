#ifndef _HARDNEWWORDITERATORCREATOR_H_
#define _HARDNEWWORDITERATORCREATOR_H_

#include "WordIterator.h"
#include "WordIteratorCreator.h"
#include "UserWordIteratorCreator.h"
#include "DiscreteWordIterator.h"
#include "EvaluateStrategy.h"
#include "UserInfo.h"
#include "IDictionary.h"
#include <random>
#include <memory>

///  生成更具挑战性的新单词迭代器
class HardNewWordIteratorCreator
    : public UserWordIteratorCreator
{
public:
    using UserWordIteratorCreator::UserWordIteratorCreator;

    std::shared_ptr<WordIterator> Create() override;
};

#endif // _HARDNEWWORDITERATORCREATOR_H_
