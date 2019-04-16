import QtQuick 2.4
import QtQuick.Controls 1.3

//歌曲列表
Rectangle {
   width: 800
   height: 500
   property PlayList playList

   TableView {
       width: parent.width
       anchors.fill: parent
       onDoubleClicked: {
           playList.setIndex(row)
       }

       rowDelegate: Rectangle {
           height: 25
           color: styleData.selected ? "#448" : (styleData.alternate ? "#eee" : "#fff")
       }

       TableViewColumn {
           role: "listIndex"
           title: " "
           width: 50
       }

       TableViewColumn {
           role: "sname"
           title: "歌曲名称"
           width: 200
       }

       TableViewColumn {
           role: "singer"
           title: "歌手"
           width: 100
       }
       model: listModel
   }

   ListModel {
       id: listModel
   }

   //更新列表
   function update(listName)
   {
       listModel.clear()
       var list = playList.getSongList(listName)
       for (var i = 0; i < list.length; i++)
       {
           list[i].listIndex = i + 1
           listModel.append(list[i])
       }
   }
}
