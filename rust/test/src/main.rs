use std::io::{stdin,stdout,Write};
fn mod3(num: i32) -> i32
{
    num % 3
}
fn main()
{
    let input = stdout().flush();
    stdin().read_line(&mut input).expect("Did not enter a correct string");
    let num = input.clone().trim().parse::<i32>().unwrap();

    while num > 3
    {
        num = mod3(num);
    }
}