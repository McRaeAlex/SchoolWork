(*  Assignment #1 *)

type DATE = (int * int * int) (* year, month, day*)
exception InvalidParameter

(* This file is where your solutions go *)

(* true if d1 comes before d2 *)
fun is_older(d1: DATE, d2: DATE): bool =
    ((#1 d1) * 10000 + (#2 d1) * 100 + (#3 d1)) < ((#1 d2) * 10000 + (#2 d2) * 100 + (#3 d2))

(* recursively goes through the list and adds 1 to the result if the month matches *)
fun number_in_month(dates: DATE list, month: int): int = 
    if null dates
    then 0
    else 
        if #2 (hd dates) = month
        then 1 + number_in_month ((tl dates), month)
        else number_in_month ((tl dates), month)

(* goes through each month and checks how many dates are in any of the months *)
fun number_in_months(dates: DATE list, months: int list): int = 
    if null months
    then 0
    else (number_in_month (dates, (hd months))) + (number_in_months (dates,(tl months)))

(* goes through each date and if it has the month adds it to a list *)
fun dates_in_month(dates: DATE list, month: int): DATE list =
    if null dates
    then []
    else 
        if (#2 (hd dates)) = month
        then hd dates :: (dates_in_month ((tl dates), month))
        else dates_in_month ((tl dates), month)

(* goes through each month and gets all the dates with the months *)
fun dates_in_months(dates: DATE list, months: int list): DATE list = 
    if null months
    then []
    else (dates_in_month (dates,(hd months))) @ (dates_in_months(dates, (tl months))) (* TODO: apparently we are supposed to use the `@` operator *)

(* returns the nth string in the list *)
fun get_nth(strs: string list, num: int): string = 
    if num = 0
    then raise InvalidParameter
    else 
        let 
        fun temp(strs: string list, num: int, count: int): string = 
            if null strs
            then raise InvalidParameter
            else
                if num = count
                then hd strs
                else temp (tl strs, num, count + 1)
        in 
        temp(strs, num, 1)
        end

(* Converts a date to the correct string *)
fun date_to_string(date: DATE): string = 
    let 
        val year = #1 date
        val month = #2 date
        val day = #3 date
        val months: string list = ["January", "Febuary", "March", "April", "May",
                               "June", "July", "August", "September", "October",
                               "November", "December"]
    in
        get_nth(months, month) ^ " " ^ Int.toString(day) ^ ", " ^ Int.toString(year)
    end

(* Goes through the list and adds each number until its over the sum*)
fun number_before_reaching_sum(sum: int, xs: int list): int = 
    let 
        fun temp(acc: int, ys: int list, count: int): int = 
            if null ys
                then raise InvalidParameter
            else 
                if (acc + (hd ys)) >= sum
                    then count
                else temp(acc + (hd ys), (tl ys), count + 1)
    in 
        temp(0, xs, 0)
    end

fun what_month(day: int): int = 
    let
        val num_days_in_months = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    in
        number_before_reaching_sum(day, num_days_in_months) + 1
    end

fun month_range(day1: int, day2: int): int list = 
    if day1 > day2
        then []
    else what_month(day1) :: month_range(day1 + 1, day2)

fun oldest(dates: DATE list): DATE option = 
    if null dates
        then NONE
    else 
        let 
            fun fold_right(acc: DATE, xs: DATE list): DATE =
                if null xs
                    then acc
                else
                    if is_older(acc, (hd xs))
                    then fold_right(acc, (tl xs))
                    else fold_right((hd xs), (tl xs))
        in 
            SOME (fold_right((hd dates), (tl dates)))
        end

fun reasonable_date(x: DATE): bool = 
    let
        fun is_leap_year(year: int):bool = 
            if not ((year mod 4) = 0)
                then false
            else if not ((year mod 100) = 0) 
                then true
            else if not ((year mod 400) = 0)
                then false
            else true
        
        fun get_nth(xs: 'a list, n: int): 'a = 
            if n = 0
            then raise InvalidParameter
            else 
                let 
                fun temp(ys: 'a list, num: int, count: int): 'a = 
                    if null ys
                    then raise InvalidParameter
                    else
                        if num = count
                        then hd ys
                        else temp (tl ys, num, count + 1)
                in 
                temp(xs, n, 1)
                end

        val leap_year_num_days_in_months = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
        val num_days_in_months = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
        val year = #1 x
        val month = #2 x
        val day = #3 x
    in
        if year < 1
            then false
        else
            if month < 1
                then false
            else if month > 12
                then false
            else 
                if day < 1
                    then false
                else if is_leap_year(year)
                    then if day > get_nth(leap_year_num_days_in_months, month)
                        then false
                    else true
                else 
                    if day > get_nth(num_days_in_months, month)
                        then false
                    else true
    end