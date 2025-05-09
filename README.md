## OpenGL Game Engine Project
This project provides an easy way to build games in a modular way using an Entity Component System (ECS).

#### Example Code
```cpp
// Add a render system which acts on entities with a transform and model
// using the lights
ecs.addSystem(Renderer(...));
ecs.addSystem(PhysicsSystem(...));

// Create a ball entity
EntityId ball = ecs.createEntity();
ecs.addComponent(ball, Transform(...));
ecs.addComponent(ball, Model(...));
ecs.addComponent(ball, Rigidbody(...));

// Create a light entity
EntityId light = ecs.createEntity();
ecs.addComponent(light, Transform(...));
ecs.addComponent(light, PointLightComponent(...));

// main loop
while(true) {
    ...
    ecs.updateSystems();
    ...
}
```

## Build Instructions

To generate project files / make files for this project using CMake run the following after cloning the project.

```
mkdir build
cd build
cmake ..build
```