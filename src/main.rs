

mod hash;
mod zpaq;

trait DataSlicer {
    fn feed(&self, data: u8) -> bool;
}

fn main() {
    println!("Hello, world!");
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