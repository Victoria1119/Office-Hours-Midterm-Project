% queries.pl - questions we ask Prolog about the schedule.
% Run:  swipl -q queries.pl

:- consult('schedule.pl').

main :-
    writeln('Q1: Who teaches COSC214?'),
    forall(taught_by('COSC214', P), format('   ~w~n', [P])),

    writeln('Q2: Which office hours are on Wednesday?'),
    forall(office_hours_on(wednesday, P, S, E),
           format('   ~w: ~w to ~w~n', [P, S, E])),

    writeln('Q3: Which days does Dr.Smith teach?'),
    forall(setof(D, teaches_on('Dr.Smith', D), Days), format('   ~w~n', [Days])),

    writeln('Q4: Where does MATH220 meet?'),
    forall(room_of('MATH220', R), format('   ~w~n', [R])),

    writeln('Q5: Who has office hours on Tuesday, and where?'),
    forall((office_hours_on(tuesday, P, S, E), office_of(P, O)),
           format('   ~w: ~w to ~w in ~w~n', [P, S, E, O])).

:- initialization((main, halt)).
