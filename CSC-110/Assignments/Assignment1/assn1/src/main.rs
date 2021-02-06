fn main() {
    println!("Hello, world!");
    printFrog();
    printOwl();
    approxPi();
}

fn printFrog() {
    println!("  @..@\n (----)\n( >__< )\n\"\"    \"\"");
}

fn printOwl() {
    println!("  ^...^\n / o,o \\\n |):::(|\n===w=w===");
}

fn approxPi() {
    let mut next_term: f32 = 1.0;
    let mut denom: f32 = 1.0;
    let mut series: f32 = 0.0;
    for i in (1..9).rev() {
        series = series + next_term/denom;
        denom = denom + 2.0;
        next_term = next_term * -1.0;
    }
    series = series * 4.0;
    println!("{}", series);
}