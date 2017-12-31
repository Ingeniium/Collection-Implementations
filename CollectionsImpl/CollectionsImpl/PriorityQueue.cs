using System;
using System.Collections.Generic;

namespace CollectionsImpl
{
    public class PriorityQueue<T>
    {
        private Tuple<float, T>[] contents;
        /*Indeces of the next indeces refers to the content's location
         in the contents array.The index's respective element refers to
         what element in the contents array precedes it in sort order.*/
        //private int[] next_indeces;
        private List<int> null_vals = new List<int>();
        private int front_index;
        private int _Count;
        public int Count
        {
            get { return _Count; }
            private set
            {
                _Count = value;
            }
        }
        /*Initializes a priority with [param] capacity.*/
        public PriorityQueue(uint capacity = 8)
        {
            contents = new Tuple<float, T>[capacity];
        }

        /*Returns how much elements the queue can currently hold.*/
        public int GetCapacity()
        {
            return contents.Length;
        }

        /*Sets the capacity of the priority queue to [param],provided that the
         arg is greater than the current capacity.*/
        public void Reserve(uint capacity)
        {
            if (capacity > GetCapacity())
            {
                Tuple<float, T>[] arr = new Tuple<float, T>[capacity];
                Array.Copy(contents, arr, GetCapacity());
                contents = arr;
            }          
        }   

        /*Sets the capacity of the priority queue to Count.If count is 0,
         than sets the capacity to 1.*/
        public void TrimExcess()
        {
            int end_threshold = Count + null_vals.Count;
            Tuple<float, T>[] arr;
            if(Count == 0)
            {
                arr = new Tuple<float, T>[1];
            }
            else
            {
                arr = new Tuple<float, T>[Count];
            }
            int arr_index = 0;
            for (int i = 0; i < end_threshold; i++)
            {
                if(contents[i] != null)
                {
                    arr[arr_index] = contents[i];
                    if(i == front_index)
                    {
                        front_index = arr_index;
                    }
                    arr_index++;
                }
            }
            null_vals.Clear();
            contents = arr;
        }

        /*Enqueue's the item with cost [param1] and places it
         in its proper place.*/
        public void Enqueue(float cost, T item)
        {
            if (Count == contents.Length)
            {
                Reserve((uint)contents.Length * 2);
            }
            Tuple<float, T> t = new Tuple<float, T>(cost, item);
            int next = Count;
            if (null_vals.Count > 0)
            {
                next = null_vals[0];
                null_vals.RemoveAt(0);
            }
            if (Count == 0 || cost < contents[front_index].Item1)
            {
                front_index = next;
            }
            contents[next] = t;
            Count++;                                             
        }

        /*Removes and returns the element at the front(which has the least
         cost) of the priority queue.*/
        public T Dequeue()
        {
            if (Count == 0)
            {
                throw new InvalidOperationException();
            }
            Count--;
            T front = contents[front_index].Item2;
            contents[front_index] = null;
            null_vals.Add(front_index);
            front_index = FindNewFront();
            return front;
        }

        /*Returns the priority of the frontmost element.*/
        public float GetFrontPriority()
        {
            if(Count == 0)
            {
                throw new InvalidOperationException();
            }
            return contents[front_index].Item1;
        }

        /*Returns the index of an element with the least cost,
         essentially finding a new frontmost element.*/
        int FindNewFront()
        {
            if (Count == 0)
            {
                return 0;
            }
            int end_threshold = Count + null_vals.Count;
            float lowest_cost = float.MaxValue;
            int index = 0;
            for (int i = 0; i < end_threshold; i++)
            {
                if (contents[i] != null)
                {
                    if (contents[i].Item1 < lowest_cost)
                    {
                        lowest_cost = contents[i].Item1;
                        index = i;
                    }
                }
            }
            return index;
        }

        /*Changes the priority of the an item provided that it is
         queued within the priority queue.Also returns the item 
         was found (and hence the priority changed)*/
        public bool ChangePriority(float cost, T item)
        {
            int end_threshold = Count + null_vals.Count;
            for(int i = 0;i < end_threshold;i++)
            {
                if(contents[i] != null && contents[i].Item2.Equals(item))
                {
                    float prev_cost = contents[i].Item1;
                    contents[i] = new Tuple<float, T>(cost, item);
                    if(front_index == i && prev_cost < cost)
                    {
                        front_index = FindNewFront();
                    }
                    return true;
                }
            }
            return false;
        }

        /*Returns whether an item is present in the queue.*/
        public bool Contains(T item)
        {
            int end_threshold = Count + null_vals.Count;
            for (int i = 0; i < end_threshold; i++)
            {
                if (contents[i] != null && contents[i].Item2.Equals(item))
                {
                    return true;
                }
            }
            return false;
        }
        
        /*Empties the priority queue.*/
        public void Clear()
        {
            int end_threshold = Count + null_vals.Count;
            for(int i = 0;i < end_threshold;i++)
            {
                contents[i] = null;
            }
            null_vals.Clear();
            Count = 0;
        }
 
    }
}


