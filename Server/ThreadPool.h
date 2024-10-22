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
class ThreadPool
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
    ThreadPool(size_t number_of_threads);

    template <class F>
    auto enqueue(F &&f) -> std::future<typename std::result_of<F()>::type>;

    ~ThreadPool();
};

// metoda enqueue pentru aduagarea unui task in coada de
// taskuri si de a-l executa asincron
template <class F>
auto ThreadPool::enqueue(F &&f) -> std::future<typename std::result_of<F()>::type>
{
    // definirea tipului de returnare a taskului
    // helpful pentru lucrul ulterior cu acesta
    using return_type = typename std::result_of<F()>::type;

    // std::packaged_task este un wrapper
    // care permite să legam un task de un std::future
    //---------------
    // std::make_shared creează un pointer inteligent catre task,
    // astfel încat să fie gestionat în siguranta
    auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));

    // obtinerea unui std::future
    // care contine rezultatul taskului si care va fi utilizat mai departe
    //"cu el lucram" de acum
    std::future<return_type> response = task->get_future();

    // adaugare de scope
    // pentru mai buna gestionare si control a alocarii variabilelor
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        // blocam accesul la coada pentru a putea adauga un task
        // adaugarea unui task in queueu
        // de inteles: --adaugarea prin apelarea taskului generat anterior
        // este un make_shared, deci deferentiem
        // iar dupa apelam pentru ca in packaged_task este "containerizata" functia

        this->tasks_queue.emplace([task]()
                                  { (*task)(); });
    }
    // exit scope:)

    // notificare threaduri ca exista a new task in the hood*()*()*)
    this->condition_var.notify_all();
    return response;
}