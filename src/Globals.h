#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <libtextdb.h>
#include <libdict.h>
#include <random>
#include <chrono>
#include <memory>

using namespace std;

#define DEFAULT_USERNAME "user"

class Globals
{
public:
    static default_random_engine RandomEngine;
    static shared_ptr<TextDB> DictDB, SentDB, LevelDB,
                              UserConfigDB, UserCounterDB, UserDictDB, UserSentDB;
    static shared_ptr<Dictionary> Dict;
    static shared_ptr<UserInfo> CurrentUser;
    static shared_ptr<WordIteratorCreator> NewWordIteratorCreator;
    static shared_ptr<WordIteratorCreator> TestWordIteratorCreator;
    static void SwitchUser(const string &UserName);
    static void Init();
    static void Open(const string &path);
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif // _GLOBALS_H_
