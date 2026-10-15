# ask_prolog.py
# Uses pyswip so Python can ask Prolog questions (like the example in the
# assignment). Prolog loads schedule.pl (made by make_prolog.py) and answers.
#
# Needs: SWI-Prolog installed, and  pip install pyswip

from pyswip import Prolog

prolog = Prolog()
prolog.consult("schedule.pl")          # load all the facts and rules

print("Who teaches COSC214?")
for r in prolog.query("taught_by('COSC214', Prof)"):
    print("  ", r["Prof"])

print("Which office hours are on Wednesday?")
for r in prolog.query("office_hours_on(wednesday, Prof, Start, End)"):
    print("  ", r["Prof"], ":", r["Start"], "to", r["End"])

print("Which days does Dr.Smith teach?")
for r in prolog.query("setof(D, teaches_on('Dr.Smith', D), Days)"):
    print("  ", r["Days"])

print("Where does MATH220 meet?")
for r in prolog.query("room_of('MATH220', Room)"):
    print("  ", r["Room"])

# A yes/no question: an empty answer list means "no"
answer = bool(list(prolog.query("teaches('Dr.Jones', 'MATH220')")))
print("Does Dr.Jones teach MATH220?", answer)
