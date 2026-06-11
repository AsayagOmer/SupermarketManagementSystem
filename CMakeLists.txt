cmake_minimum_required(VERSION 3.30)
project(Supermarket-System C)

set(CMAKE_C_STANDARD 17)

add_executable(DigitalStore
    main.c
    date.h
    date.c
    utils.c
    utils.h
    product.h
    product.c
    customer.h
    customer.c
    shopping_cart.h
    shopping_cart.c
    shoppingitem.h
    shoppingitem.c
    supermarket.h
    supermarket.c
    vtable.h
    vtable.c
    fileio.h
    fileio.c
    analytics.h
    analytics.c)
