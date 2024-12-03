## Important change log 

- e5756ca4c8fed9f99b2006351ee6edd6c2f8daaf Is the last commit that supports Not using the shader. 


## What is this project?

A Force-Directed Graph, or Force-Based Graph, is a type of layout commonly used in a variety of application areas: network visualization, large graph visualization, knowledge representation, system management, or mesh visualization.

This contains an implementation of the force directed graph in unreal engine 5.

replicate the result of the popular library https://github.com/vasturiano/3d-force-graph.

![image](https://github.com/user-attachments/assets/4e648745-8ee3-4fc7-9008-fd1d53785711)


## Documentation

### Some options to use

connect_to_previous: bool
    If true, the node will be connected to the previous node. Which make the graph a snake like structure.









## Main reference of this project

Dependency link

https://vasturiano.github.io/3d-force-graph/example/large-graph/ -> https://github.com/vasturiano/three-forcegraph -> https://github.com/vasturiano/d3-force-3d -> https://github.com/vasturiano/d3-octree

## To do list

[a relative link](To_do_list.md)


## Reference

### https://github.com/thomaswall/ue4-force-graph

I regard this as an improved version of https://github.com/thomaswall/ue4-force-graph

It is improved in the following ways
1. It is Using the latest version of the unreal engine 5 instead of the unreal engine 4.24
2. It is using a different implementation of the charge force
3. It corrects some of the Mistakes such as calculating the bias of the link.

It is worse in the following ways
1. The implementation of the charge force, which is the many body force, use a custom implementation of the OCtree, Which seems to be slower than the library version of the unreal engine.





