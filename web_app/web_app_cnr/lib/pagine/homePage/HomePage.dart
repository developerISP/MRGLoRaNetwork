import 'package:flutter/material.dart';

import 'tabs/quartoTab.dart';

import 'bodypart/bodypart.dart';
import 'drawer/DrawerPart.dart';

MaterialColor colore = Colors.red;

class HomePage extends StatefulWidget {
  HomePage({Key key}) : super(key: key);

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  void callback(MaterialColor col) {
    setState(() {
      colore = col;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      drawer: Drawer(
        child: DrawerPart()

      ),

      appBar: AppBar(
        title: Text("progetto CNR", style: TextStyle(fontSize: 24),),
        centerTitle: true,

        actions: <Widget>[
          FlatButton(
            child: Text(
              "project info",
              style: TextStyle(
                fontSize: 15,
                color: Colors.white,
              ),
            ),
            onPressed: () {
              Navigator.push(
              context, MaterialPageRoute(builder: (context) => QuartoTab()));
            },
          )
        ],
        
        elevation: 0,
      ),

      //_body part of the page
      body: ListView(
        children: <Widget>[
          
          BodyPart(),

        ],
      )
    );
  }
}