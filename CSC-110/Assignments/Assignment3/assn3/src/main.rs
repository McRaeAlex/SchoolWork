use std::io;
//use std::str::FromStr;

fn main() {
    // Transportation
    // --------------
    println!("How many km do you drive in one day?");
    let mut km = String::new();
    io::stdin().read_line(&mut km)
        .expect("Issues occured when reading");
    let mut efficency = String::new();
    print!("Choose the appropriate fuel efficency for your car:\n\tvery small car : 7.1 km/litre\n\tsmall car : 8.2 km/litre\n\tsports car : 14.9 km/litre\n\tSUV : 12.3 km/litre\nOr, if you know the efficency rating, choose another number:\n");
    io::stdin().read_line(&mut efficency)
        .expect("Issues occured when reading");    
    // always remeber to trim the string or float parse errors will occur
    let kpd = km.trim().parse::<f32>().unwrap();
    let effic = efficency.trim().parse::<f32>().unwrap(); 
    let carbon_car = transportationFootprint(kpd, effic);
    println!("your carbon footprint with respect to transportation is {}kg/year", carbon_car);

    // Electricity
    // -----------
    println!("What is your average electricity consumption per month, in kilowatts?");
    let mut elect_consumption = String::new();
    io::stdin().read_line(&mut elect_consumption)
        .expect("Error: issues occured when reading");
    println!("How many people live in the house?");
    let mut people = String::new();
    io::stdin().read_line(&mut people)
        .expect("Error: issues occured when reading");
    let num_people = people.trim().parse::<i32>().unwrap();
    let e_consumption = elect_consumption.trim().parse::<f32>().unwrap();
    let eFootPrint = electricityFootprint(e_consumption, num_people);
    println!("Your carbon footprint with respect to electricity is {}kg/year", eFootPrint);
    
    // Food
    // ----
    println!("Of all the food you eat, what percent consists of meat or fish?");
    let mut meat = String::new();
    io::stdin().read_line(&mut meat)
        .expect("Issue when reading");
    println!("Of all the food you eat, what percent consists of dairy?");
    let mut dairy = String::new();
    io::stdin().read_line(&mut dairy)
        .expect("Issue when reading");
    println!("Of all the food you eat, what percent consists of fruit or vegtables?");
    let mut fruit = String::new();
    io::stdin().read_line(&mut fruit)
        .expect("Issue when reading");
    println!("Of all the food you eat, what percent consists of cabohydrates?");
    let mut carbs = String::new();
    io::stdin().read_line(&mut carbs)
        .expect("Issue when reading");
    let num_meat = meat.trim().parse::<f32>().unwrap();
    let num_dairy = dairy.trim().parse::<f32>().unwrap();
    let num_fruit = fruit.trim().parse::<f32>().unwrap();
    let num_carbs = carbs.trim().parse::<f32>().unwrap();
    let foodprint = foodFootprint(num_meat, num_dairy, num_fruit, num_carbs);
    println!("Your carbon footprint with respect to food is {}kg/year.", foodprint);
    let total = totalFootprint(carbon_car, eFootPrint, foodprint);
    println!("your total consumption is: {}kg/year", total);

    
}

fn transportationFootprint(kPD: f32, effic: f32) -> f32 {
    (2.3 * (365.0 * (kPD/effic)))
}

fn electricityFootprint(kw_hours: f32, num: i32) -> f32 {
    ((kw_hours * 12.0 * 0.257)/num as f32)
}

fn foodFootprint(meat: f32, dairy: f32, fv: f32, carb: f32) -> f32 {
    (meat * 53.1 + dairy * 13.8 + fv * 7.6 + carb* 3.1)/100.0
}

fn totalFootprint(trans: f32, elec: f32, food: f32) -> f32 {
    ((trans + elec + food)/1000.0)
}
