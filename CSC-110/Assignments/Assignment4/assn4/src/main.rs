fn main() {
    guess_my_birthday();
}

fn month_to_string(month: i32) -> &'static str {
    match month {
        1 => "Janurary",
        2 => "Febuary",
        3 => "March",
        4 => "April",
        5 => "May",
        6 => "June",
        7 => "July",
        8 => "August",
        9 => "September",
        10 => "October",
        11 => "November",
        12 => "December",
        _ => "",
    }
}

fn month_to_int(month: &str) -> i32 {
    match month {
        "Janurary" => 1,
        "Febuary" => 2,
        "March" => 3,
        "April" => 4,
        "May" => 5,
        "June" => 6,
        "July" => 7,
        "August" => 8,
        "September" => 9,
         "October" => 10,
         "November" => 11,
         "December" => 12,
        _ => 0,
    }
}

fn is_leap_year(year: i64) -> bool {
    if year%4 == 0 && (year%100 != 0 || year%400 == 0) {
        true
    } else {
        false
    }
}

fn num_days_in_month(month: i32, year: i64) -> i32 {
    let month_days = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    if month == 2 {
        if is_leap_year(year) {
            29
        }
        else {
            28
        }
    } else {
        month_days[(month-1) as usize]
    }
}

fn guess_my_birthday() {
    let mut birthday = String::new();
    std::io::stdin().read_line(&mut birthday)
        .expect("Cannot read line");
    let month = get_month();
    let date = get_date(month);  
    println!("your birthday is {} {}", month, date);
}

fn get_month() -> &'static str {
    let mut low = 1;
    let mut high = 12;
    while high >= low {
        println!("high: {} Low: {}", high, low);
        let middle: i32 = (low + high) / 2;
        let month = month_to_string(middle);
        println!("Is your birthday in {}? yes or no:", month);
        let mut temp = String::new();
        std::io::stdin().read_line(&mut temp)
            .expect("Error could not read");
        if temp.trim() == "yes" {
            return month;
        }
        temp = String::from("");
        println!("Is your birthday after this month? yes or no:");
        std::io::stdin().read_line(&mut temp)
            .expect("Could not read");
        if temp.trim() == "yes" {
            low = middle + 1;
        }
        if temp.trim() == "no" {
            high = middle - 1;
        }
    }
    "you lied"
}

fn get_date(month: &str) -> i32 {
    let mut low = 1;
    let mut high = num_days_in_month(month_to_int(month), 1);
    while high >= low {
        let middle: i32 = (low + high) / 2;
        println!("Is your birthday in {}? yes or no:", middle);
        let mut temp = String::new();
        std::io::stdin().read_line(&mut temp)
            .expect("Error could not read");
        if temp.trim() == "yes" {
            return middle;
        }
        temp = String::from("");
        println!("Is your birthday after this day? yes or no:");
        std::io::stdin().read_line(&mut temp)
            .expect("Could not read");
        if temp.trim() == "yes" {
            low = middle + 1;
        }
        if temp.trim() == "no" {
            high = middle - 1;
        }
    }
    0
}