(* Assign 03 Provided Code *)

(*  Version 1.0 *)

exception NoAnswer

datatype pattern = Wildcard
		 | Variable of string
		 | UnitP
		 | ConstP of int
		 | TupleP of pattern list
		 | ConstructorP of string * pattern

datatype valu = Const of int
	      | Unit
	      | Tuple of valu list
	      | Constructor of string * valu

(* Description of g:
g takes a function f1 which handles the wildcards
g takes a f2 which handles the variables
g evaluates a expression
*)

fun g f1 f2 p =
    let
	val r = g f1 f2
    in
	case p of
	    Wildcard          => f1 ()
	  | Variable x        => f2 x
	  | TupleP ps         => List.foldl (fn (p,i) => (r p) + i) 0 ps
	  | ConstructorP(_,p) => r p
	  | _                 => 0
    end


(**** put all your code after this line ****)

fun only_capitals(ss: string list): string list = 
	List.filter (fn s => Char.isUpper(String.sub(s, 0))) ss


fun longest_string1(ss: string list): string =
	List.foldl (fn (acc, s) => if String.size(s) >= String.size(acc)
		then s
		else acc)
		""
		ss

fun longest_string2(ss: string list): string =
	List.foldl (fn (acc, s) => if String.size(s) > String.size(acc)
		then s
		else acc)
		""
		ss

fun longest_string_helper g xs = 
	List.foldl (fn (acc, x) => 
					if g(String.size(acc), String.size(x))
					then x
					else acc) "" xs

val longest_string3 = fn xs => longest_string_helper (fn (acc, x) => x >= acc) xs 

val longest_string4 = fn xs => longest_string_helper (fn (acc, x) => x > acc) xs

val longest_capitalized = fn xs : string list => 
	(longest_string1 o only_capitals) xs

fun rev_string s = 
	(String.implode o List.rev o String.explode) s

fun first_answer g xs =
	let
		fun f x = case x of SOME x => true | NONE => false;
	in
		case (List.filter f o List.map g) xs of 
			SOME v :: xs' => v
			| _ => raise NoAnswer
	end

fun all_answers g xs =
	let 
		val m = SOME [];
		fun f (acc: 'b list option, x: 'b list option): 'b list option =
			case acc of 
				NONE => NONE
			  | SOME v => (case x of
							    NONE => NONE
							  | SOME i => SOME (v @ i));
	in
	    ((List.foldr f m) o (List.map g)) xs
	end

fun count_wildcards p =
	let 
		fun f1 c = 1
		fun f2 c = 0
	in g f1 f2 p end

fun count_wild_and_variable_lengths p =
	let
		fun f1 c = 1
		fun f2 s = String.size(s)
	in g f1 f2 p end

fun count_some_var (s, p) = 
	let 
		fun f1 c = 0
		fun f2 c = if s = c then 1 else 0
	in g f1 f2 p end

fun check_pat p =
	let 
		fun get_strings (p: pattern): string list =
			case p of
					Wildcard        => []
				| Variable x        => [x]
				| TupleP ps         => List.foldl (fn (i, acc) => acc @ get_strings(i)) [] ps
				| ConstructorP(_,p) => get_strings p
				| _                 => []

		fun unique_list xs =
			case xs of 
				[] => true
				| x::xs' => if List.exists (fn c => c = x) xs'
							then false
							else unique_list xs'
	in 
		(unique_list o get_strings) p
	end

fun match (v: valu, pat: pattern): (string * valu) list option =
	let 
		fun tuple_thing (ps: pattern list, vs: valu list) : (string * valu) list option = 
			all_answers (fn (p, v) => match(v, p)) (ListPair.zip(ps, vs))
	in 
		case (pat, v) of
			(Wildcard, value) => SOME []
		|   (Variable s, value) => SOME [(s, value)]
		|   (UnitP, Unit) => SOME []
		|	(ConstP s, Const value) => if s = value then SOME [] else NONE
		|   (TupleP ps, Tuple vs) => if List.length ps = List.length vs then tuple_thing(ps, vs) else NONE
		|   (ConstructorP(s1, p), Constructor(s2, v)) => if s1 = s2 then match (v, p) else NONE
		| 	_ => NONE
	end


fun first_match v pl = 
	SOME (first_answer (fn elem => match(v, elem)) pl)
	handle NoAnswer => NONE

