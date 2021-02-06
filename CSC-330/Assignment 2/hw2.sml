(* if you use this function to compare two strings (returns true if the same
   string), then you avoid some warning regarding polymorphic comparison  *)

fun same_string(s1 : string, s2 : string) =
    s1 = s2

(* put your solutions for Part 1 here *)
fun all_except_option(str: string, xs: string list): string list option =
    let fun inner(str: string, xs: string list): bool * string list = 
        case xs of
              [] => (false, [])
            | x::xs' => if same_string(x, str) then (true, xs')
                        else (let val (found, ys) = inner(str, xs') in (found, x::ys) end)
    in 
        case inner(str, xs) of 
              (true, ys) => SOME ys 
            | (false, ys) => NONE
    end

fun get_substitutions1(xs: string list list, str: string): string list = 
    case xs of
          [] => []
        | x::xs' => case all_except_option(str, x) of
                         SOME list => list @ get_substitutions1(xs', str)
                       | NONE => get_substitutions1(xs', str)

fun get_substitutions2(xs: string list list, str: string): string list =
    let
        fun acc(arr: string list, ss: string list list): string list = 
            case ss of
                [] => arr
                | x::xs' => (case all_except_option(str, x) of
                                NONE => acc(arr, xs')
                                | SOME ls => acc(arr @ ls, xs'))

    in 
        acc([], xs)
    end

type NAME = {first:string, middle:string, last:string}

fun similar_names(xs: string list list, name: NAME): NAME list = 
    let 
        val {first=firstname, middle=middlename, last=lastname} = name
        fun create_list_of_names(xs: string list, middlename: string, lastname: string): NAME list = 
            case xs of
                 [] => []
                |x::xs' => {first=x, middle=middlename, last=lastname} :: create_list_of_names(xs', middlename, lastname)
    in 
        create_list_of_names(firstname :: get_substitutions1(xs, firstname), middlename, lastname)
    end

(************************************************************************)
(* Game  *)

(* you may assume that Num is always used with valid values 2, 3, ..., 10 *)

datatype suit = Clubs | Diamonds | Hearts | Spades
datatype rank = Jack | Queen | King | Ace | Num of int
type card = suit * rank

datatype color = Red | Black
datatype move = Discard of card | Draw


exception IllegalMove

(* put your solutions for Part 2 here *)
fun card_color((s, _): card): color = 
    case s of 
          Clubs => Black
        | Spades => Black
        | _ => Red

fun card_value((_, r): card): int = 
    case r of 
          Ace => 11
        | Num n=> n (* Matches numbers *)
        | _ => 10 (* Matches kings, queens, jacks *)

fun remove_card(cs: card list, c: card, e: exn): card list = 
    let 
        fun acc(a: card list, cs: card list): card list =
            case cs of
                  [] => raise e
                | x::xs => if c = x 
                           then a @ xs 
                           else acc(a @ [x], xs)
    in
        acc([], cs)
    end

(* I believe in this case this is the best option *)
fun all_same_color([]): bool = true 
|   all_same_color(c::cs: card list): bool = 
    let
        fun temp(xs: card list, clr: color): bool =
            case xs of
                  [] => true
                | x::xs' => if card_color(x) = clr
                               then temp(xs', clr)
                               else false
    in 
        temp(cs, card_color(c))
    end

fun sum_cards(cs: card list): int =
    let 
        fun acc(sum: int, cs: card list): int =
            case cs of
                  [] => sum
                | x::xs => acc(sum + card_value(x), xs)
    in 
        acc(0, cs)
    end

fun score(hand: card list, goal: int): int = 
    let 
        val sum = sum_cards(hand)
        fun end_score(pre_score: int): int = 
            if all_same_color(hand)
            then pre_score div 2
            else pre_score
    in
        if sum > goal
        then end_score((sum - goal) * 2)
        else end_score(goal - sum)
    end

fun officiate(cs: card list, ms: move list, goal: int): int = 
    let
    fun card_in_hand(hand: card list, c: card): card list option = 
        let fun inner(hand: card list): bool * card list =
            case hand of
                  [] => (false, hand)
                | x::xs => if x = c then (true, xs)
                            else let val (found, ys) = inner(xs) 
                            in (found, x::ys) 
                            end
        in
            case inner(hand) of
                 (true, new_hand) => SOME new_hand
                |(false, _) => NONE
        end

    fun logic(hand: card list, cs: card list, ms: move list): int = 
        case ms of
              [] => score(hand, goal)
            | Discard c :: ms' => (case card_in_hand(hand, c) of
                                      SOME new_hand => logic(new_hand, cs, ms')
                                    | NONE => raise IllegalMove)
            | Draw :: ms' => (case cs of
                                  [] => score(hand, goal)
                                | crd :: cs' => 
                                    let 
                                        val draw_hand = crd::hand
                                    in 
                                        if sum_cards(draw_hand) > goal
                                        then score(draw_hand, goal)
                                        else logic(draw_hand, cs', ms') 
                                    end)
    in
    logic([], cs, ms)
    end

(* test cases *)
val test1_0 = all_except_option("wow", ["these", "seem", "unnessecary"]) = NONE;
val test2_0 = get_substitutions1([["Maria","Musa"],["Maria","Betty"],["Maria"]],
                               "Maria")
            = ["Musa","Betty"];
val test3_0 = get_substitutions2([["Maria","Musa"],["Maria","Betty"],["Maria"]],
                               "Maria")
            = ["Musa","Betty"];
val test4_0 = similar_names([
                             ["Thomas", "Maria"],
                             ["Batman", "Maria","Bruce"],
                             ["Maria"], []
                         ], {first="Maria", middle = "(whoknows)", last="Wayne"}) =
            [{first="Maria",last="Wayne",middle="(whoknows)"},
             {first="Thomas",last="Wayne",middle="(whoknows)"},
             {first="Batman",last="Wayne",middle="(whoknows)"},
             {first="Bruce",last="Wayne",middle="(whoknows)"}];
val test5_0 = card_color((Hearts, Num 2)) = Red;
val test6_0 = card_value((Hearts, Num 2)) = 2;
exception shouldntHappen
val test7_0 = remove_card([(Hearts, Num 2)], (Hearts, Num 2), shouldntHappen) = [];
val test8_0 = all_same_color([(Hearts, Num 2), (Spades, Num 4), (Hearts, Num 3)]) = false;
val test9_0 = sum_cards([(Hearts, Num 2), (Spades, Num 4), (Hearts, Num 3)]) = 9;
val test10_0 = score([(Hearts, Num 2), (Spades, Num 4), (Hearts, Num 3)], 1) = 16;
val test11_0 = officiate([(Hearts, Num 2), (Spades, Num 4), (Hearts, Num 3)], 
                         [Draw, Draw, Draw, Draw], 1) = 1;