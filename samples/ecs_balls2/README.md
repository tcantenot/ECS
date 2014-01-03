# Sample App 2

## Description

Thousands of colored bouncing balls.

    R : Toogle rendering
    M : Toogle movement
    Right/Left arrow : Add/Remove 100 balls

## Comments

Second try with ECS.

In this app, I tried to reduce cache misses by using systems inheriting from EntitySystem instead of EntityProcessingSystem
and by iterating through arrays instead of querying for entities one by one.

Results:

    175000 balls at 60fps.

A lot of improvements made but does not equals OO code speed yet...
