using System;
using System.Collections.Generic;
using System.Text;
namespace CollectionsImpl
{
    public class PriorityQueue<Key, Value> where Key : IComparable<Key>
    {
        private Tuple<Key, Value>[] contents;
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
            contents = new Tuple<Key, Value>[capacity];
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
                Tuple<Key, Value>[] arr = new Tuple<Key, Value>[capacity];
                Array.Copy(contents, arr, GetCapacity());
                contents = arr;
            }
        }

        /*Enqueue's the item with cost [param1] and places it
         in its proper place.*/
        public void Enqueue(Key key, Value v)
        {
            Count++;
            if (Count == contents.Length)
            {
                Reserve((uint)contents.Length * 2);
            }
            contents[Count] = new Tuple<Key, Value>(key, v);
            int index = Count;
            int parent = index / 2;
            while (contents[parent] != null 
                && contents[parent].Item1.CompareTo(key) > 0)
            {
                Tuple<Key, Value> temp = contents[parent];
                contents[parent] = contents[index];
                contents[index] = temp;
                index = parent;
                parent = index / 2;
            }

        }

        /*Removes and returns the element at the front(which has the least
         cost) of the priority queue.*/
        public Value Dequeue()
        {
            if (Count == 0)
            {
                throw new InvalidOperationException();
            }
            Value front = contents[1].Item2;
            contents[1] = contents[Count];
            int index = MinChild(1);
            int parent = 1;
            while (index <= Count && contents[index] != null
                && contents[parent].Item1.CompareTo(
                    contents[index].Item1) > 0)
            {
                Tuple<Key, Value> temp = contents[parent];
                contents[parent] = contents[index];
                contents[index] = temp;
                parent = index;
                index = MinChild(parent);
            }
            Count--;
            return front;
        }

        private int MinChild(int parent)
        {
            Tuple<Key, Value> left = parent * 2 <= Count ?
                contents[parent * 2] : null;
            Tuple<Key, Value> right = parent * 2 + 1 <= Count
                ? contents[parent * 2 + 1] : null;
            if (left == null || right == null 
                || (left.Item1.CompareTo(right.Item1) < 0))
            {
                return parent * 2;
            }
            return parent * 2 + 1;
        }

        /*Returns the priority of the frontmost element.*/
        public Key GetFrontPriority()
        {
            if (Count == 0)
            {
                throw new InvalidOperationException();
            }
            return contents[1].Item1;
        }

        /*Returns the index of an element with the least cost,
         essentially finding a new frontmost element.*/
        public Value GetFrontValue()
        {
            if (Count == 0)
            {
                throw new InvalidOperationException();
            }
            return contents[1].Item2;
        }

        /*Returns whether an item is present in the queue.*/
        public bool Contains(Value item)
        {
            for (int i = 1; i <= Count; i++)
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
            for (int i = 1; i <= Count; i++)
            {
                contents[i] = null;
            }
            Count = 0;
        }

        public override string ToString()
        {
            StringBuilder str = new StringBuilder();
            for (int i = 1; i <= Count; i++)
            {
                str.Append("(" + contents[i].Item1 + ", " + contents[i].Item2 + ") ");
            }
            return str.ToString();
        }

    }
}
