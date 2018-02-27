 The en
# Collection-Implementations
This contains implementations of containers I made either for classwork or for on my own time.

2/25/2018 - RedBlackTree
--------------------------
This was also something I did in inspiration of the ringbuffer assignment I had for my first programming class at UT Austin. This was written in C++. Although the container is unfinished as it lacks a remove method and a output stream method to show the contents in order,
It comes with a guaranteed logarithmic time min and max functions for both the key and value. It also has functions like reserve and trim excess to give users control over how much capacity they want to set aside.The Insert is also guaranteed logarithmic time.

12/30/2017 - PriorityQueue
--------------------------
It took me a month to implement this before scrapping my initial approach which was still valid but much slower than the current 
implementation.This was done entirely on my free time due to the inspiration I got from the RingBuffer class I had to write for my Intro to Programming class I had last fall semester. The enqueue is in constant time,while both dequeue and contains are in linear time since this implementation didn't contain nodes.
