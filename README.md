# Hybrid::ECS

## Entity-component system (ECS) framework in C++11

This entity system is based on the architecture of Coment ([https://github.com/Biracun/Coment](https://github.com/Biracun/Coment)) and [Artemis](http://gamadu.com/artemis/).


## Folders

### include && src

Sources of the ECS.

### samples

Thousands of colored bouncing balls.

    R : Toogle rendering
    M : Toogle movement
    Right/Left arrow : Add/Remove 100 balls


These are several versions of the same app using different "paradigms":

* *oo_balls*   : classic object-oriented code
* *ecs_balls*  : entity-component system code
* *ecs_balls2* : entity-component system code improved

### tests

A bunch of unit tests testing the ECS framework.

## Compilation instructions

### Linux

    make
