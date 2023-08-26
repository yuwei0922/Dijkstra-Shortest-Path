# Dijkstra Shortest Path
The project implemented the Dijkstra algorithm to solve the shortest path problem from a source vertex to other vertices in a directed graph. Additionally, the results were visualized for better comprehension.
## Modular Design and Implementation of the Algorithm
### 1. Design of Structures and Reading CSV Data Files
Designed two structures: City and Route, which are used to store city and route information respectively.

### 2. Graph Creation Using Adjacency Matrix
Given 199 vertices and only 1975 edges, forming a sparse graph, I chose the adjacency matrix representation to trade some space for time efficiency.

### 3. Graph Traversal (Depth-First Search)
Implemented a recursive DFS() function that visits all vertices in a connected graph.

### 4. Shortest Path Calculation and Display (A to B)
Employed Dijkstra's algorithm for finding the shortest path. The default shortest path is based on minimum time. To obtain the path with minimum cost, the parameter "time" can be replaced with "cost".

### 5. Visualization of the Shortest Path on a Map
Utilized HTML to generate an HTML file named "graph.htm", which displays the points and routes of the shortest path.




