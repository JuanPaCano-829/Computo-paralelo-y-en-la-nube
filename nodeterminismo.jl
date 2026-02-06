using .Threads

function thread_counting(n)
    a = zeros(Int, 1)
    """para multiples hilos dentro de un solo proceso"""
    lk = ReentrantLock()
    @threads for i=1:n
        lock(lk) do 
            a[1] += 1
        end
        println("soy hilo ", 
            Threads.threadid(), 
            "valor de a[1] = ", 
            a[1], 
            "\n")
    end
    return a[1]
end

function simple_counting(n)
    """Julia no arranca con zeros tenemos que ponerle unos 
    Este vector es para pasar la referencia al vector 
    
    """
    a = zeros(Int, 1)
    for i=1:n
        a[1] += 1
    end
    return a[1]
end

function dimsinucion(n)
    @time begin
        valor = 10000000
        for i in 1:n
            valor -= 1
        end
    end
end

function dimsinucion2(n)
    @time begin
        valor = 10000000
        @threads for i in 1:n
            valor -= 1
        end
    end
end

function dimsinucion3(n)
    @time begin
        lk = ReentrantLock()
        valor = 10000000
        @threads for i in 1:n
            lock(lk) do 
                valor -= 1
            end
        end
    end
end

function dimsinucion4(n)
    @time begin
        valor = 10000000
        @threads for i in 1:n
            10000000 - i
        end
    end
end