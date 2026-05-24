cmake_minimum_required(VERSION 3.30)
project(DigitalStore C)

set(CMAKE_C_STANDARD 11)

add_executable(DigitalStore
    main.c
        Date.h
        Date.c
        utils.c
        utils.h
        Product.h
        customer.h
        shopping_cart.c
        supermarket.h
        supermarket.c
        product.c
        customer.c
        shoppingitem.c)
