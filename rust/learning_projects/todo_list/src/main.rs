use std::collections::HashSet;
use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufReader, Write};
use std::path::Path;

struct List {
    // String is what is displayed and the i8 is used for 
    // looking at and indexing entries
    entries: Vec<(String, i8)>,
}

impl List {
    fn new() -> Self {
        List {
            entries: Vec::new(),
        }
    }

    // Pushed back the supplied string to the end of the entries array
    fn add_entry(&mut self, entry: String) {
        self.entries.push((entry, self.entries.len() as i8)); 
    }

    // Saves the list to the file system so it can be accesed later
    fn save_list(&self, filename: &str) -> io::Result<()> {
        let mut file = File::create(filename)?;
        for (name, value) in &self.entries {
            writeln!(file, "{},{}", name, value)?;
        }
        Ok(())
    }

    // Uses previouly mentioned i8 to pop an index out of the entries vector
    fn remove_entry(&mut self, entry: i8) {
        if let Some(pos) = self.entries.iter().position(|(_, v)| *v == entry) {
            self.entries.remove(pos - 1);
        } else {
            eprintln!("Entry with index {} not found", entry);
        }
    }

    // Reads a list from a file, used for the --load option to load a list from a file
    fn read_list(filename: &str) -> io::Result<List> {
        let path = Path::new(filename);
        let file = File::open(&path)?;
        let reader = BufReader::new(file);

        let mut list = List::new();
        for line in reader.lines() {
            let line = line?;
            let mut parts = line.split(',');
            if let (Some(name), Some(value)) = (parts.next(), parts.next()) {
                let value = value.parse::<i8>().unwrap_or(-1);
                list.entries.push((name.trim().to_string(), value));
            }
        }

        Ok(list)
    }

    // Swaps 2 values in the entries array
    fn move_entry(&mut self, old_pos: i8, new_pos: i8) {
        let old_pos_usize = old_pos as usize - 1;
        let new_pos_usize = new_pos as usize - 1;

        // Ensure positions are within bounds
        if old_pos_usize >= self.entries.len() || new_pos_usize >= self.entries.len() {
            eprintln!("Error: Position out of bounds.");
            return;
        }

        // Extract the entry from the old position
        let entry = self.entries.remove(old_pos_usize);

        // Insert the entry at the new position
        // Note: new_pos_usize might be greater than old_pos_usize after removal,
        // so use insertion with an index check.
        if new_pos_usize > self.entries.len() {
            // If the new position is beyond the current end of the list
            self.entries.push(entry);
        } else {
            self.entries.insert(new_pos_usize, entry);
        }

        // Update indices of all entries to be continuous and correct
        self.repair();
    }

    // Ensures that no index numbers are skipped and that there are no duplicate indeces
    fn repair(&mut self) {
        // Create a set to track unique names and a map to track the original indices
        let mut unique_names = HashSet::new();
        let mut to_remove = Vec::new(); // Collect indices of duplicates to remove

        // Identify duplicates
        for (index, (_, name)) in self.entries.iter().enumerate() {
            if unique_names.contains(name) {
                to_remove.push(index);
            } else {
                unique_names.insert(name.clone());
            }
        }

        // Remove duplicates
        for index in to_remove.iter().rev() {
            self.entries.remove(*index);
        }

        // Reindex remaining entries to ensure continuous indices without gaps
        for (index, (_, name)) in self.entries.iter_mut().enumerate() {
            *name = index as i8;
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect(); // Args provided to the program
    let mut list = List::new();              // New list for modifications to be made

    let mut i = 1; // Index counter for the args vector
    while i < args.len() { // Loop through all the args
        match args[i].as_str() { // Match each args to a valid option or throw error
            "--save" | "-s" => {
                if i + 1 < args.len() { // Ensure filename arg exists
                    let filename = &args[i + 1]; // Set filename to the arg after --save
                    if let Err(e) = list.save_list(filename) { // Make sure saving doesnt throw an IO error
                        eprintln!("Error saving list: {}", e); // Throw error if there was an IO error
                    }
                    i += 2; // Move past the filename
                } else { // If there isnt an arg after --save, tell the user
                    eprintln!("No filename provided after --save");
                    i += 1;
                }
            }
            "--load" | "-l" => {
                if i + 1 < args.len() { // Ensure filename arg exists
                    let filename = &args[i + 1]; // Set filename to the arg after --load
                    match List::read_list(filename) { // Read the list to the program using the filename arg
                        Ok(loaded_list) => { // If read succesfully:
                            list = loaded_list; // Set the main program list to the files contents
                        }
                        Err(e) => { // If reading error:
                            eprintln!("Error reading list: {}", e); // Inform the user of the error
                        }
                    }
                    i += 2; // Move past the filename
                } else { // Inform the user of a missing filename arg
                    eprintln!("No filename provided after --load");
                    i += 1;
                }
            }
            "--display" | "-d" => { 
                for (name, value) in &list.entries { // Loop through the entries array in list

                    println!("{}. {}", value + 1, name); // Print each value 
                }
                i += 1;
            }
            "--add" | "-a" => {
                if i + 1 < args.len() { // Ensure the input exists
                    list.add_entry(args[i + 1].clone()); // Clones args, not sure why it wants me to do this but sure
                    i += 2; // Above line also adds a new entry to the end of the entries vector
                } else { // Inform the user if they didnt give text input to add
                    eprintln!("No value provided for --add");
                    i += 1;
                }
            }
            "--remove" | "-r" => {
                if i + 1 < args.len() { // Ensure the required input number exists
                    if let Ok(index) = args[i + 1].parse::<i8>() { // Parse args into an i8 for reading indeces
                        list.remove_entry(index); // Remove an entry based on its index number
                        i += 2;
                    } else { // Inform the user if they didnt give a proper number
                        eprintln!("Invalid index provided for --remove");
                        i += 1;
                    }
                } else { // Inform the user if they didnt give any input
                    eprintln!("No value provided for --remove");
                    i += 1;
                }
            }
            "--repair" | "-re" => {
                list.repair(); // Runs a repair to ensure theres no duplicate or skipped indeces
                i += 1;
            }
            "--move" | "-m" => {
                if i + 2 < args.len() { // Ensure the next 2 args exist
                    // Parses the next two args into i8 for index use. Also checks for errors at the same time
                    if let (Ok(old_index), Ok(new_index)) = (args[i + 1].parse::<i8>(), args[i + 2].parse::<i8>()) {
                        list.move_entry(old_index, new_index); // Move the entry using the above values
                        i += 3; // Move past both indices
                    } else { // Inform the user if they didnt give valid numbers
                        eprintln!("Invalid indices provided for --move");
                        i += 1;
                    }
                } else { // Inform the user if they didnt give any input
                    eprintln!("No values provided for --move");
                    i += 1;
                }
            }

            "--help" | "-h" => { // Prints the help text, quick explanations and examples for the commands
                print!("Usage:\nNote that every args pipes its output to the next meaning you can load, chain some adds or removes then save in the same command
                --save, -s   \tSaves the list to the filename,                           -s filename
                --load, -l   \tLoads the specified filename to the process,              -l filename
                --add, -a    \tAdds an entry to the loaded list,                         -a \"entry\"
                --remove, -r \tRemoves an entry from the loaded list using index number, -r 1
                --move, -m   \tSwaps the entries with each other                         -m 1 2
                --display, -d\tDisplays the values of the loaded list,                   -d
                --repair, -re\tRepairs the list by fixing duplicate index numbers,       -re\n");
                i += 1;
            }
            _ => { // If the argument isnt valid then tell the user
                eprintln!("Unknown argument: {}", args[i]);
                i += 1;
            }
        }
    }
}
