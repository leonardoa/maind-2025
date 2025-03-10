CREATE OR REPLACE FUNCTION update_updated_at_column()
RETURNS TRIGGER AS $$
BEGIN
  NEW.updated_at = now();
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER update_realtimedatabase7_updated_at
BEFORE UPDATE ON realtimedatabase7  /* table name */
FOR EACH ROW
EXECUTE FUNCTION update_updated_at_column();