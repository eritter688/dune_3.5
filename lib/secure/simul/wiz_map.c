//
//  wiz_map
//


protected int wiz_map (object ob){
  return member(load_object ("/secure/access/wiz_map")->query_map ()[ob->query_level ()], ob->query_real_name ()) > -1;    }
