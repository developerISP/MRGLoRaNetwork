import 'package:flutter/material.dart';
import '../tabs/terzoTab.dart';

class DrawerPart extends StatelessWidget {
  const DrawerPart({Key key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Column(
      

      children: <Widget>[
        

        DrawerHeader(

          child: Container(
            alignment: Alignment.center,
            child: Text("CNR web application", style: TextStyle(fontSize: 20, fontWeight: FontWeight.w500)),
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

              FlatButton(
                onPressed: () {
                  Navigator.pop(context);
                },
                onLongPress: () => null,
                child: Text("prima pagina"),
                splashColor: Colors.lightBlue[50],
              ),

              FlatButton(
                onPressed: () {
                  Navigator.pop(context);
                },
                onLongPress: () => null,
                child: Text("seconda pagina"),
                splashColor: Colors.lightBlue[50],
              ),

              FlatButton(
                onPressed: () {
                  Navigator.push(context, MaterialPageRoute(builder: (context) => TerzoTab()));
                },
                onLongPress: () => null,
                child: Text("terza pagina"),
                splashColor: Colors.lightBlue[50],
              ),


            ],
          )
        )
        

      ],
    );

  }
}