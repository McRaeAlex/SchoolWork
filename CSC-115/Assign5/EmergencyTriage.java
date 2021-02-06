/**
 * EmergencyTriage.java
 * Provided the necessary information here,
 * making sure to delete these lines.
 */

// This is the beginning of the source code to help you get started.
// Do not change the following:

public class EmergencyTriage {
	private Heap<ERPatient> waiting;

	/**
	 * The empty EmergencyTriage is initialized.
	 */
	public EmergencyTriage() {
		waiting = new Heap<>();
	}

	/**
	 * A patient is registed at triage. The triage nurse creates a record for 
	 * the patient and the patient is then added to the triage queue.
	 * @param lastName - The patient's last name.
	 * @param firstName - The patient's first and subsequent names.
	 * @param triageCategory - The triage category assigned to the patient by 
	 * the triage nurse.
	 */
	public void register(String lastName, String firstName, String triageCategory) {
		this.waiting.insert(new ERPatient(lastName, firstName, triageCategory));
	}

	/**
	 * Removes the highest priority patient from triage to enter the Emergency Room.
	 * @return The record of the patient that is moving into the Emergency Room.
	 */
	public ERPatient admitToER() {
		return this.waiting.removeRootItem();
	}

	/**
	 * Retrieves the record of the patient who currently has the highest 
	 * priority. The patient's record remains in the queue.
	 * @return The record of the patient who is next
	 */
	public ERPatient whoIsNext() {
		ERPatient tmp = this.waiting.getRootItem();
		this.waiting.insert(tmp);
		return tmp;
	}

	/**
	 * @return The number of patients waiting but not yet admitted
	 */
	public int numberOfPatientsWaiting() {
		return this.waiting.size();
	}

	/**
	 * Used for internal testing only.
	 * @param args - Not used.
	 */
	public static void main(String[] args) {
		EmergencyTriage test = new EmergencyTriage();
		System.out.println("Testing register\n------------------------------------\n");
		test.register("McRae", "Alex", "Life-threatening");
		test.register("a", "b", "Ambulatory");
		test.register("c", "d", "Life-threatening");
		test.register("e", "f", "Major fracture");
		test.register("g", "h", "Acute");
		test.register("i", "j", "Chronic");
		test.register("k", "l", "Life-threatening");

		System.out.println("\nTesting whoisnext\n---------------------------------\n");
		System.out.println("Should be the same thing twice and should be me");
		System.out.println(test.whoIsNext());
		System.out.println(test.whoIsNext());
		
		System.out.println("\nTesting admitToER\n---------------------------------\n");
		System.out.println("Should be me again and then c d");
		System.out.println(test.admitToER());
		System.out.println(test.admitToER());

		System.out.println("\nTesting numberOfPatientsWaiting\n-------------------------------------\n");
		System.out.println(test.numberOfPatientsWaiting());
		System.out.println(test.admitToER());
		System.out.println(test.numberOfPatientsWaiting());
		System.out.println(test.admitToER());
		System.out.println(test.admitToER());
		System.out.println(test.admitToER());
		System.out.println(test.admitToER());
		System.out.println(test.admitToER());
		System.out.println(test.admitToER());
	}

}