use std::io;
use std::str::FromStr;

fn main() {
    areaCircle();
}

fn printMessage() {
    let mut message = String::new();
    io::stdin().read_line(&mut message)
        .expect("Failed to read line");
    let num = message.chars().count() as u32;
    message = message.trim().to_string();
    let mut i: u32 = 0;
    while i < num + 2 {
        print!("*");
        i = i + 1;
    }
    println!("*");
    println!("* {} *", &message);
    i = 0;
    while i < num + 2 {
        print!("*");
        i = i + 1;
    }
    println!("*");
}

fn printOwl() {
    printMessage();
    println!("   \\\n    \\\n     ^...^\n    / o,o \\\n    |):::(|\n   ===w=w===");
}

fn areaCircle() {
    let mut next_term: f32 = 1.0;
    let mut denom: f32 = 1.0;
    let mut series: f32 = 0.0;
    for i in (1..10001).rev() {
        series = series + next_term/denom;
        denom = denom + 2.0;
        next_term = next_term * -1.0;
    }
    series = series * 4.0;
    let mut userin = String::new();
    io::stdin().read_line(&mut userin)
        .expect("Error cannot read line");

    userin = userin.trim().to_string();
    let radius = f32::from_str(&userin).unwrap();
    println!("The area of the circle is {}", radius * radius * series);
}