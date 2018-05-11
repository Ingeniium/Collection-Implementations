
# Collection-Implementations
This contains implementations of containers I made on my own time.

5/10/2018 - PriorityQueue(C#)
------------------------
I decided to revisit my priority queue. The previous implementation was had a O(N) Dequeue to make up for
the O(1) Enqueue it had. I wanted to try writing a heap (for the purpose of expanding my knowledge and understanding)
, so now both operations are Log(n) at their worst.

5/5/2018 - RedBlackTree(C++)
------------------------
I believe I have finally finished the tree. I added a remove method, and a get height method. Furthermore,there was 
a significant change to the restructuring code for insertion that was crucial for the tree to be balanced. I made
the mistake previously of thinking that the control variable of a switch statement could be evalutated multiple times.
This mistake actually caused the tree to have instances of red parent-child pairs that served to imbalance the tree.
This has been fixed.

4/27/2018 - RedBlackTree(C++)
------------------------
I done away with the array based storage system I only did for educational purposes.Furthermore, I made code changes that shortened the length of some methods and reduced redundancy.Semester got busy,so I had to take a temporary break from personal projects.I plan to finish this container by the end of May(after finals/when I get back home).Also, duplicates are no longer allowed.

3/17/2018 - RedBlackTree(C++)
------------------------
I reworked the find method, added contains method, a method to print all the elements in order, and a method to return a sorted heap array of both the keys an the elements (of course,they are copied so that no change is done to the tree itself). I also reworked the iterator to do more preconditional checking and throw more exceptions when it fails.

2/25/2018 - RedBlackTree(C++)
------------------------
This was also something I did in inspiration of the ringbuffer assignment I had for my first programming class at UT Austin. This was written in C++. Although the container is unfinished as it lacks a remove method and a output stream method to show the contents in order,
It comes with a guaranteed logarithmic time min and max functions for both the key and value. It also has functions like reserve and trim excess to give users control over how much capacity they want to set aside.The Insert is also guaranteed logarithmic time.

12/30/2017 - PriorityQueue(C#)
--------------------------
It took me a month to implement this before scrapping my initial approach which was still valid but much slower than the current 
implementation.This was done entirely on my free time due to the inspiration I got from the RingBuffer class I had to write for my Intro to Programming class I had last fall semester. The enqueue is in constant time,while both dequeue and contains are in linear time since this implementation didn't contain nodes.
