
trait HashCode {
  fn asInt(&self) -> u32;
}

trait HashFunction<T: HashCode> {
  fn reset(&mut self);
  fn update(&mut self, data: &[u8]);
  fn finalize(&mut self) -> T;
}

pub struct U32HashCode {
  hash: u32,
}

impl U32HashCode {
  pub fn with(value: u32) -> U32HashCode {
    U32HashCode {
      hash: value,
    }
  }
}

impl HashCode for U32HashCode {
  fn asInt(&self) -> u32 {
    self.hash
  }
}