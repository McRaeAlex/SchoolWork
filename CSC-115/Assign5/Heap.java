
/**
 * Heap.java
 * Provide the necessary header information here, 
 * making sure to delete these lines.
 */

import java.util.List;
import java.util.ArrayList;
import java.util.NoSuchElementException;

// This is the beginning of the source code to help you get started.
// Do not change the following:

public class Heap<E extends Comparable<E>> {

	private ArrayList<E> heap;
	private int size;
	private static final int CAPACITY = 100;

	/**
	 * Creates an empty heap.
	 */
	public Heap() {
		heap = new ArrayList<E>(CAPACITY);
		for (int i = 0; i < CAPACITY; i++) {
			heap.add(null);
		}
	}

	/**
	 * true if the heap is empty.
	 */
	public boolean isEmpty() {
		return this.size == 0;
	}

	/**
	 * The number of elements in the heap.
	 */
	public int size() {
		return this.size;
	}

	/**
	 * Adds an element to the heap, placing it into the only correct position.
	 * @param element - The element to insert.
	 */
	public void insert(E element) { // figure out if we need min of max
		this.heap.set(this.size, element);
		int place = this.size();
		int parent = (place - 1) / 2;
		while (parent >= 0 && this.heap.get(place).compareTo(this.heap.get(parent)) < 0) {
			E temp = this.heap.get(place);
			this.heap.set(place, this.heap.get(parent));
			this.heap.set(parent, temp);
			place = parent;
			parent = (place - 1) / 2;
		}
		this.size++;
	}

	/**
	 * Removes the element that is currently at the root (or top) of the heap.
	 * @return E - The element.
	 * @throws NoSuchElementException - if the heap is empty.
	 */
	public E removeRootItem() throws NoSuchElementException {
		if (!this.isEmpty()) {
			E toReturn = this.heap.get(0);
			int loc = --this.size;
			this.heap.set(0, this.heap.get(loc)); // get the last item
			this.heap.set(loc, null);
			this.heapRebuild(0);
			return toReturn;
		} else {
			throw new NoSuchElementException("The heap is empty");
		}
	}

	/**
	 * 
	 */
	public E getRootItem() throws NoSuchElementException {
		if (!this.isEmpty()) {
			return this.heap.get(0);
		} else {
			throw new NoSuchElementException("THe heap is empty");
		}
	}

	public void heapRebuild(int root) {
		int child = 2 * root + 1;
		if (child < this.size()) {
			int rightChild = child + 1;
			if (rightChild < this.size() 
				&& this.heap.get(rightChild).compareTo(this.heap.get(child)) < 0) {
				child = rightChild;
			}

			if (this.heap.get(root).compareTo(this.heap.get(child)) > 0) {
				E temp = this.heap.get(root);
				this.heap.set(root, this.heap.get(child));
				this.heap.set(child, temp);
				this.heapRebuild(child);
			}
		}
	}

	/**
	 * Used for internal testing only
	 */
	private void toArr () {
		System.out.println(this.heap.toString());
	}

	/**
	 * Used for internal testing only.
	 * @param args - Not used.
	 */
	public static void main(String[] args) {
		Heap<String> test = new Heap<String>();
		System.out.println("Testing is Empty");
		System.out.println("Should return true");
		System.out.println(test.isEmpty());
		System.out.println("Testing insert");
		System.out.println("The array should look like this [\"0\"] and is Empty should return false");
		test.insert("0");
		test.toArr();
		test.insert("5");
		test.toArr();
		test.insert("56");
		test.toArr();
		test.insert("65");
		test.toArr();
		System.out.println(test.isEmpty());
		System.out.println("Testing Size");
		System.out.println("Should return 4");
		System.out.println(test.size());
		System.out.println("Testing getRootItem");
		System.out.println("Should return 65");
		System.out.println("Returns: " + test.getRootItem());
		test.toArr();
		System.out.println(test.size());
		System.out.println("Should return 56");
		System.out.println("Returns: " + test.getRootItem());
		test.toArr();
		System.out.println(test.size());
	}
}