#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t number_of_threads)
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

ThreadPool::~ThreadPool()
{
    {
        // blocam mutex-ul pentru a seta flag-ul de oprire
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        this->stop = true;
    }
    // notificam toate thread-urile pentru a le permite sa se opreasca
    this->condition_var.notify_all();
    // wait ca toate thread-urile sa se opreasca, cu: (join)
    for (std::thread &thread__ : this->all_threads_vec)
    {
        thread__.join();
    }
}
