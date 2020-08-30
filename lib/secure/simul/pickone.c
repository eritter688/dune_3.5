// pickone.h
// Mreh, October 22, 2006
// For picking one element out of something.
//  Ported to DUNE_3.5 unchanged. - Math

mixed pickone (mixed arg)
{
  if (!arg)
    raise_error ("No argument passed to pickone.\n");

  switch (get_type_info (arg)[0])
  {
    case 0:	// Invalid
      raise_error ("Illegal type (unknown) to pickone.\n");
      break;
    case 1:	// LValue
      raise_error ("Illegal type (lvalue) to pickone.\n");
      break;
    case 2:	// Integer
      return (arg > 0 ? random (arg) : -random (abs (arg)));
    case 3:	// String
      return (arg != "" ? explode (arg, "")[random (strlen (arg))] : "");
    case 4:	// Array (pointer)
      return (sizeof (arg) ? arg [random (sizeof (arg))] : "");
    case 5:	// Object
      raise_error ("Illegal type (object) to pickone.\n");
      break;
    case 6:	// Mapping
      return (sizeof (m_indices (arg)) ? m_indices (arg)[random (sizeof (m_indices (arg)))] : "");
      break;
    case 7:	// Float
      raise_error ("Illegal type (float) to pickone.\n");
      break;
    case 8:	// Closure
      raise_error ("Illegal type (closure) to pickone.\n");
      break;
    case 9:	// Symbol
      raise_error ("Illegal type (symbol) to pickone.\n");
      break;
    case 10:	// Quoted Array
      raise_error ("Illegal type (quoted array) to pickone.\n");
      break;
    default:
      raise_error ("Unknown type to pickone.\n");
      break;
  }
}
