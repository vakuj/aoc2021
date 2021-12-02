# Task day 2

## Summary

From https://adventofcode.com/2021/day/2

---

### Part 1

What do you get if you multiply your final horizontal position by your final depth?

### Example

* forward 5 adds 5 to your horizontal position, a total of 5.
* down 5 adds 5 to your depth, resulting in a value of 5.
* forward 8 adds 8 to your horizontal position, a total of 13.
* up 3 decreases your depth by 3, resulting in a value of 2.
* down 8 adds 8 to your depth, resulting in a value of 10.
* forward 2 adds 2 to your horizontal position, a total of 15.

After following these instructions, you would have a horizontal position of 15 and a depth of 10. (Multiplying these together produces 150.)

---

### Part 2

* down X increases your aim by X units.
* up X decreases your aim by X units.
* forward X does two things:
    - It increases your horizontal position by X units.
    - It increases your depth by your aim multiplied by X.

Using this new interpretation of the commands, calculate the horizontal position and depth you would have after following the planned course.
What do you get if you multiply your final horizontal position by your final depth?

### Example

* forward 5 adds 5 to your horizontal position, a total of 5. Because your aim is 0, your depth does not change.
* down 5 adds 5 to your aim, resulting in a value of 5.
* forward 8 adds 8 to your horizontal position, a total of 13. Because your aim is 5, your depth increases by 8*5=40.
* up 3 decreases your aim by 3, resulting in a value of 2.
* down 8 adds 8 to your aim, resulting in a value of 10.
* forward 2 adds 2 to your horizontal position, a total of 15. Because your aim is 10, your depth increases by 2*10=20 to a total of 60.

After following these new instructions, you would have a horizontal position of 15 and a depth of 60. (Multiplying these produces 900.)

___


