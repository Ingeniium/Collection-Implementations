using System;
using System.Collections.Generic;
using System.Diagnostics;

class TestPriorityQueue
{
    public static void Main()
    {
        TestGetFrontValue();
        Console.WriteLine("Press any key to continue to the next test.");
        Console.ReadKey();
        TestDequeue();
        Console.WriteLine("Press any key to exit the application.");
        Console.ReadKey();
    }

    public static void TestGetFrontValue()
    {
        CollectionsImpl.PriorityQueue<int, int> queue = new CollectionsImpl.PriorityQueue<int, int>();
        const int SIZE = 100;
        const int NUM_TESTS = 1000;
        Random rand = new Random();
        double wrong = 0;
        for (int i = 0; i < NUM_TESTS; i++)
        {
            int min = int.MaxValue;
            for (int j = 0; j < SIZE; j++)
            {
                int num = rand.Next() % SIZE;
                if (num < min)
                    min = num;
                queue.Enqueue(num, num);
            }
            if (queue.GetFrontValue() != min)
            {
                wrong++;
                Console.WriteLine(queue);
            }
            queue.Clear();
        }
        Console.WriteLine("Out of " + NUM_TESTS + " tests, " + wrong
            + " tests did not produce the correct minimum, leaving a" +
            " GetFrontValue accuracy of " + (NUM_TESTS - wrong) / NUM_TESTS * 100 + "%.");

    }

    public static void TestDequeue()
    {
        CollectionsImpl.PriorityQueue<int, int> queue = new CollectionsImpl.PriorityQueue<int, int>();
        List<int> list = new List<int>();
        const int SIZE = 100;
        const int NUM_TESTS = 1000;
        int wrong = 0;
        Random rand = new Random();
        for (int i = 0; i < NUM_TESTS; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                int num = rand.Next() % SIZE;
                list.Add(num);
                queue.Enqueue(num, num);
            }
            list.Sort();
            for(int j = 0;j < list.Count;j++)
            {
                if (queue.Dequeue() != list[j])
                    wrong++;
            }
            queue.Clear();
            list.Clear();
        }
        Console.WriteLine("Out of " + NUM_TESTS * SIZE + " values, " + wrong
           + " values weren't placed correctly, leaving a" +
           " Dequeue accuracy of " + (NUM_TESTS * SIZE - wrong) / (NUM_TESTS * SIZE) * 100 + "%.");
    }

}

