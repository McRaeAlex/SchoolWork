/**
 * Remeber to put in comments at the end 
 */

public class AdmittedPatients {
    protected TreeNode root;

    /**
     * Creates an empty BinarySearchTree, ready to admit patients.
     */
    public AdmittedPatients() {
        this.root = null;
    }

    /**
     * Enters the record of an admitted patient into the current 
     * BinarySearchTree, maintaining the ordering of the tree. Previous records 
     * in the tree are not moved from their position. The ordering is determined
     * by the compareTo method of the HospitalPatient class.
     * Parameters: 
     * @param patient - The patient that has been admitted.
     */
    public void admit(HospitalPatient patient) {
        if (this.isEmpty()) {
            this.root = new TreeNode(patient, null, null, null);
        } else {
            TreeNode compare = this.root;
            while (true) {
                int cmp = compare.item.compareTo(patient);
                if (cmp > 0) {
                    if (compare.left == null) {
                        compare.left = new TreeNode(patient, null, null, compare);
                        break;
                    } else {
                        compare = compare.left;
                    }
                } else if (cmp < 0) {
                    if (compare.right == null) {
                        compare.right = new TreeNode(patient, null, null, compare);
                        break;
                    } else {
                        compare = compare.right;
                    }
                } else if (cmp == 0) {
                    System.out.println("Two patients are equal");
                    break;
                }
            }
        }
    }

    /**
     * Retrieves the information about a patient, given an id number. The 
     * patient's record remains in its current location in the tree.
     * Parameters:
     * @param id - The id of the patient.
     * Returns:
     * @return The complete record of that patient or null if the patient is not
     * in hospital.
     */
    public HospitalPatient getPatientInfo(String id) {
        if (this.isEmpty()) {
            return null;
        } else {
            TreeNode compare = this.root;
            while (true) {
                int cmp = compare.item.getId().compareTo(id);
                if (cmp > 0) {
                    if (compare.left == null) {
                        return null;
                    } else {
                        compare = compare.left;
                    }
                } else if (cmp < 0) {
                    if (compare.right == null) {
                        return null;
                    } else {
                        compare = compare.right;
                    }
                } else if (cmp == 0) {
                    return compare.item;
                }
            }
        }
    }

    /**
     * Removes a patient's record from the BinarySearchTree. If the record is 
     * not there, nothing changes. If the record is in a node with no children, 
     * then that node is removed from the tree. If the record is in a node with 
     * one child, then that child replaces the removed node in the tree (it 
     * becomes the child of the removed node's parent) If the record is in a 
     * node with two children, the the tree is adjusted as described on page 609
     * of the textbook. Essentially, it is replaced by it's inorder successor.
     * Parameters:
     * @param patient - The patient record to remove
     */
    public void discharge(HospitalPatient patient) {
        this.root = this.deleteItem(this.root, patient);
    }

    private TreeNode deleteItem(TreeNode comparNode, HospitalPatient patient) {
        if (!this.isEmpty()) {
            int cmp = comparNode.item.compareTo(patient);
            if (cmp == 0) {
                return this.deleteNode(comparNode);
            } else if (cmp > 0) {
                comparNode.left = this.deleteItem(comparNode.left, patient);
                return comparNode;
            } else if (cmp < 0) {
                comparNode.right = this.deleteItem(comparNode.right, patient);
                return comparNode;
            }
        }
        return null;
    }

    private TreeNode deleteNode(TreeNode delete) {
        // if its a leaf
        if (delete.left == null && delete.right == null) {
           return null;
        } else if (delete.left == null) {
            return delete.right;
        } else if (delete.right == null) {
            return delete.left;
        } else {
            delete.item = this.findLeftMost(delete.right);
            delete.right = this.deleteLeftMost(delete.right);
            return delete;
        }
    }

    /**
     * finds the left most node
     */
    private HospitalPatient findLeftMost(TreeNode a) {
        if (a.left == null) {
            return a.item;
        } else {
            return findLeftMost(a.left);
        }
    }

    /**
     * deletes the left most node
     */
    private TreeNode deleteLeftMost(TreeNode a) {
        if (a.left == null) {
            return a.right;
        } else {
            a.left = deleteLeftMost(a.left);
            return a;
        }
    }

    /**
     * Prints out a list of patient locations in alphabetical order, one entry 
     * per line. Formatting is defined by the getLocation method of 
     * HospitalPatient.
     */
    public void printLocations() {
        inorder(this.root);
    }

    /**
     * does a inorder traversal of the tree
     */
    private void inorder(TreeNode a) {
        if (a != null) {
            inorder(a.left);
            System.out.println(a.item.getLocation());
            inorder(a.right);
        }
    }

    /**
     * checks to see if the tree is empty
     */
    public boolean isEmpty() {
        return this.root == null;
    }

    /**
     * makes the tree empty
     */
    public void makeEmpty() {
        this.root = null;
    }

    public static void main(String[] args) {
        AdmittedPatients admitted = new AdmittedPatients();
        HospitalPatient p1 = new HospitalPatient(new SimpleDate(2018, 2, 27), "Duck", "Donald", 'C', 123);
        HospitalPatient p2 = new HospitalPatient(new SimpleDate(2018, 1, 15), "Mouse", "Minnie", 'B', 307);
        HospitalPatient p3 = new HospitalPatient(new SimpleDate(2018, 3, 1), "Dog", "Goofie", 'A', 422);
        HospitalPatient p4 = new HospitalPatient(new SimpleDate(2017, 12, 25), "Newman", "Alfred", 'X', 111);
        HospitalPatient p5 = new HospitalPatient(new SimpleDate(2017, 12, 29), "Alex", "McRae", 'X', 111);
        admitted.admit(p1);
        admitted.admit(p4);
        admitted.admit(p3);
        admitted.admit(p2);
        admitted.admit(p5);
        admitted.printLocations();
        admitted.discharge(p4);
        admitted.discharge(p1);
        // these two lines cause the tree to be viewed in a little
        // resizable window.      
        ViewableTree dbt = new ViewableTree(admitted);
        dbt.showFrame();
    }
}