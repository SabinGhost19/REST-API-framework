#include "ThreadPool.h"

ThreadPoll::ThreadPoll(size_t number_of_threads)
{
    for (size_t i = 0; i < number_of_threads; i++)
    {
        // crearea thread si adaugare in vectorul de threaduri
        this->all_threads_vec.emplace_back([this]
                                           {
                // definirea buclei infinite pentru verificarea
                // existentei unui task disponibil
                while (true)
                {
                    std::function<void()> task;
                    {
                        // blocarea mutex pentru a putea accesa task queue
                        // fara a intmpina race conditions
                        std::unique_lock<std::mutex> lock(this->queue_mutex);

                        // asteptarea pana cand nu mai exista un task in coada
                        // sau trebuie sa oprim executia intregului proces
                        // utilizarea unei functii lambda
                        this->condition_var.wait(lock, [this]
                                                 { return !this->tasks_queue.empty() || stop; });

                        // iesire din while daca trebuie oprit
                        // sau nu exista taskuri in coada
                        if (stop && this->tasks_queue.empty())
                            return;

                        // preluam taskul din coada
                        task = std::move(this->tasks_queue.front());
                        this->tasks_queue.pop();
                    }
                        try
                        {
                            // executie task si tratare a erorilor
                            task();
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << "Error executing task: " << e.what() << std::endl;
                        }
                        catch (...)
                        {
                            std::cerr << "Unknown error occurred while executing task." << std::endl;
                        }
                    } });
    }
}

template <class F>
// metoda enqueue pentru aduagarea unui task in coada de
// taskuri si de a-l executa asincron
auto ThreadPoll::enqueue(F &&f) -> std::future<typename std::result_of<F()>::type>
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

 ThreadPoll::~ThreadPool() {
        {
            // blocam mutex-ul pentru a seta flag-ul de oprire
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->stop = true;
        }
        // notificam toate thread-urile pentru a le permite sa se opreasca
        this->condition_var.notify_all();
        // wait ca toate thread-urile sa se opreasca, cu: (join)
        for (std::thread &thread__ : this->all_threads_vec) {
            thread__.join();
        }
    }