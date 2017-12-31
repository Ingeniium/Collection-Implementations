using System;
using System.Collections.Generic;
using System.Diagnostics;

class TestPriorityQueue
{
    public static void Main()
    {
        Test1();
        Console.WriteLine("Press any key to continue to the next test.");
        Console.ReadKey();
        Console.Clear();
        Test2();
        Console.WriteLine("Press any key to continue to the final test.");
        Console.ReadKey();
        Console.Clear();
        Test3();
        Console.WriteLine("Press any key to exit the application.");
        Console.ReadKey();
    }

    public static void Test1()
    {
        uint capacity = (uint)Math.Pow(10, 6);
        CollectionsImpl.PriorityQueue<bool> queue = new CollectionsImpl.PriorityQueue<bool>(capacity);
        Console.WriteLine("Queue Count is " + queue.Count);
        Console.WriteLine("Queue capacity is " + queue.GetCapacity());
        Random rand = new Random();
        const int NUM_VALUES = 8;
        for (int i = 0; i < NUM_VALUES; i++)
        {
            int cost = rand.Next(100);
            Console.WriteLine("Enequeuing a bool with a cost of " + cost);
            queue.Enqueue(cost, false);
        }
        Console.WriteLine();
        Console.WriteLine("Trimming excessive capacity");
        queue.TrimExcess();
        Console.WriteLine("Queue Count is " + queue.Count);
        Console.WriteLine("Queue capacity is " + queue.GetCapacity());
        Console.WriteLine();
        Console.WriteLine("Trimming after each dequeue hereafter...");
        for (int i = 0; i < queue.Count; i++)
        {
            Console.WriteLine("Dequeuing a bool with a cost of " + queue.GetFrontPriority());
            queue.Dequeue();
            queue.TrimExcess();
            Console.WriteLine("Capacity is now " + queue.GetCapacity());
            i--;
        }
        Console.WriteLine();
        Console.WriteLine("Enqueuing true with a cost of 1.5 and false with a cost of 2...");
        queue.Enqueue(1.5f, true);
        queue.Enqueue(2, false);
        Console.WriteLine("Changing true's priority to 100...");
        queue.ChangePriority(100, true);
        for (int i = 0; i < queue.Count; i++)
        {
            Console.WriteLine("Dequeuing a bool with a cost of " + queue.GetFrontPriority());
            queue.Dequeue();
            i--;
        }
    }

    public static void Test2()
    {
        uint capacity = (uint)Math.Pow(10, 6);
        CollectionsImpl.PriorityQueue<bool> queue = new CollectionsImpl.PriorityQueue<bool>();
        Random rand = new Random();
        Stopwatch timer = new Stopwatch();
        Console.WriteLine("Enqueuing " + capacity + " boolean values with their own costs...");
        timer.Start();
        for (int i = 0; i < capacity; i++)
        {
            queue.Enqueue(rand.Next((int)capacity), true);
        }
        timer.Stop();
        Console.WriteLine("Time elapsed: " + timer.ElapsedMilliseconds + " milliseconds for queue");
        List<Tuple<float, bool>> list = new List<Tuple<float, bool>>();
        timer.Reset();
        Console.WriteLine("Adding " + capacity + " tuples of type float, bool into a list and then sorting once...");
        timer.Start();
        for (int i = 0; i < capacity; i++)
        {
            list.Add(new Tuple<float, bool>(rand.Next((int)capacity), true));
        }
        list.Sort();
        timer.Stop();
        Console.WriteLine("Time elapsed: " + timer.ElapsedMilliseconds + " milliseconds for list");
    }

    public static void Test3()
    {
        uint capacity = (uint)Math.Pow(10, 6);
        CollectionsImpl.PriorityQueue<bool> queue = new CollectionsImpl.PriorityQueue<bool>(capacity);
        Random rand = new Random();
        Stopwatch timer = new Stopwatch();
        for (int i = 0; i < queue.GetCapacity(); i++)
        {
            queue.Enqueue(rand.Next((int)capacity), true);
        }
        timer.Start();
        queue.Dequeue();
        timer.Stop();
        Console.WriteLine("Dequeing an element of a queue of " + capacity + " elements took " + timer.ElapsedMilliseconds + " milliseconds.");
    }

}
