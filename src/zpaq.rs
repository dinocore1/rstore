
use crate::hash::HashFunction;
use crate::hash::U32HashCode;
use std::num::Wrapping;

struct ZpaqHash {
  hash: Wrapping<u32>,
  last_byte: u8,
  predicted_byte: [u8;256],
}

impl ZpaqHash {

  fn new() -> ZpaqHash {
    ZpaqHash {
      hash: Wrapping(0),
      last_byte: 0,
      predicted_byte: [0;256],
    }
  }

  fn feed(&mut self, c: u8) -> u32
  {

    let factor;
    if c == self.predicted_byte[self.last_byte as usize] {
      factor = Wrapping(314159265);
    } else {
      factor = Wrapping(271828182);
    }


    self.hash = (self.hash + Wrapping(c as u32) + Wrapping(1)) * factor;
    self.predicted_byte[self.last_byte as usize] = c;
    self.last_byte = c;
    self.hash.0
  }

}

impl HashFunction<U32HashCode> for ZpaqHash {

  fn reset(&mut self) {
    self.hash = Wrapping(0);
    self.last_byte = 0;
    self.predicted_byte = [0;256];
  }

  fn update(&mut self, data: &[u8]) {
    for c in data.iter() {
      self.feed(*c);
    }
    
  }

  fn finalize(&mut self) -> U32HashCode {
    U32HashCode::with(self.hash.0)
  }
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn zpaq_update() {

    let mut f = ZpaqHash::new();
    f.update(&[1, 2, 3, 4]);

    let hashcode = f.finalize();

    

  }

}