function azarparalela(lanzamientos)
    # @distributed (+) divide el bucle entre los procesos y suma (+) los resultados
    numcaras = @distributed (+) for i = 1:lanzamientos
        Int(rand(Bool))
    end
    return numcaras
end
