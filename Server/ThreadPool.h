#include <thread>             // Biblioteca pentru utilizarea thread-urilor
#include <vector>             // Biblioteca pentru utilizarea containerului vector (pentru a stoca thread-urile)
#include <queue>              // Biblioteca pentru utilizarea cozii (queue) pentru a stoca task-urile
#include <mutex>              // Biblioteca pentru utilizarea mutex-urilor (pentru sincronizare)
#include <condition_variable> // Biblioteca pentru utilizarea variabilelor condiționale (pentru sincronizare)
#include <functional>         // Biblioteca pentru utilizarea std::function (pentru a stoca task-urile)
#include <iostream>           // Biblioteca pentru utilizarea input/output-ului
#include <netinet/in.h>       // Biblioteca pentru structurile și funcțiile necesare pentru rețea
#include <unistd.h>           // Biblioteca pentru funcțiile POSIX, cum ar fi close()
#include <future>             // Biblioteca pentru utilizarea funcționalităților asincrone, precum std::async
#include <string>             // Biblioteca pentru utilizarea clasei std::string
#include <cstring>            // Biblioteca pentru funcția memset
#include <unordered_map>      // Biblioteca pentru utilizarea containerului unordered_map

// gestionarea pool de threaduri pentru
// a executa taskurile fiecarui client concurent
class ThreadPoll
{
private:
    //-------
    // vectorul de threaduri pe care le cream in constructor
    // si asteapta sa existe takuri in coada si sa le execute
    // in starea idle initial
    std::vector<std::thread> all_threads_vec;

    //-------
    // stocarea taskurilor sub forma unor functii (sunt defapt functii:))))
    std::queue<std::function<void()>> tasks_queue;

    //-------
    // mutex pentru a gestiona accesul la queue de taskuri
    // smth like: citeste si coada si face pop la task doar un thread nu mai multe
    std::mutex queue_mutex;

    //-------
    // variabila pentru notificarea threadurilor
    std::condition_variable condition_var;

    //-------
    // little flag pentru a opri executia tuturor threadurilor
    bool stop = false;

public:
    ThreadPoll(size_t number_of_threads);
    template <class F>
    auto enqueue(F &&f) -> std::future<typename std::result_of<F()>::type>;
    // Destructorul ThreadPool - oprește toate thread-urile și le alătură (join)
    ~ThreadPool();
};