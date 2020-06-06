import 'package:flutter/material.dart';

///Define all the section about the drawer or sideBar
class DrawerPart extends StatelessWidget {

  ///generate the drawer (or the sideBar)
  const DrawerPart({Key key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Column(
      
      children: <Widget>[
        
        DrawerHeader(

          child: Container(
            alignment: Alignment.bottomLeft,
            child: Text("HomePage", style: TextStyle(fontSize: 20, fontWeight: FontWeight.w500)),
          ),

          decoration: BoxDecoration(
            gradient: LinearGradient(
              colors: <Color>[Colors.lightBlueAccent, Colors.lightBlue],
              begin: Alignment.topLeft,
              end: Alignment.bottomRight
            )
          ),
        ),

        Expanded(
          flex: 8,
          child: ListView(
            children: <Widget>[

              ListTile(
                onTap: () {
                },
                onLongPress: () => null,
                
                leading: Icon(Icons.insert_chart),
                title: Text("overview ozon chart"),
                trailing: Icon(Icons.arrow_right),

                
                
              ),

              ListTile(
                onTap: () {
                  Navigator.pop(context);
                },
                onLongPress: () => null,
                
                leading: Icon(Icons.insert_chart),
                title: Text("overview temperature chart"),
                trailing: Icon(Icons.arrow_right),

                
              ),

              ListTile(
                onTap: () {
                  Navigator.pop(context);
                },
                onLongPress: () => null,
                
                leading: Icon(Icons.insert_chart),
                title: Text("overview umidity chart"),
                trailing: Icon(Icons.arrow_right),

              ),

              ListTile(
                onTap: () {
                  Navigator.pop(context);
                },
                onLongPress: () => null,
                
                leading: Icon(Icons.insert_chart),
                title: Text("overview wind chart"),
                trailing: Icon(Icons.arrow_right),

              ),

            ],
          )
        )
        

      ],
    );

  }
}