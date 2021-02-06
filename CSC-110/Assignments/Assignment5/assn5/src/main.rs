use std::io;

fn main() {
    println!("Hello, world!");
    let array: [String] = [String::from("FUCK")];
}

fn findLongest (arr: &[String]) -> String {
    let mut longest: String = String::from("");
    for x in arr.iter() {
        if x.len() > longest.len() {
            longest = x.to_string();
        }
    }
    return longest;
}