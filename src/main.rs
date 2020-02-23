

mod hash;
mod zpaq;

use std::env;
use getopts::Options;

trait DataSlicer {
    fn feed(&mut self, data: u8) -> bool;
}

fn print_usage(program: &String, opts: &Options) {
    let brief = format!("Usage: {} -i file", program);
    println!("{}", opts.usage(&brief));
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let program = &args[0];

    let mut opts = Options::new();
    opts.reqopt("i", "input", "input file path", "input file or - to read from stdin");
    opts.optflag("h", "help", "print usage");
    let matches = match opts.parse(&args[1..]) {
        Ok(m) => m,
        Err(f) => panic!(f.to_string()),
    };

    if matches.opt_present("h") {
        print_usage(program, &opts);
        return;
    }

    let input_file = matches.opt_str("i");
    println!("input file: {}", input_file.expect("wtf"));
    
    
}


#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn test_rolling() {

    /*
    let splitter = Zpaq::with_average_size(20);
    let mut hasher = ZpaqHash::default();

    let r = hasher.feed('3' as u8);

    let mut i = 0;
    
    if r < splitter.max_hash && 
        !splitter.range.under_min(&i) &&
        splitter.range.exceeds_max(&i)
    {
        println!("split");
    }
    */

    
    

    



  }
}