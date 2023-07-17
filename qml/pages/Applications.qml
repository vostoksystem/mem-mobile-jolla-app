import QtQuick 2.2
import Sailfish.Silica 1.0
import "../composents"
import "../hints"

Page {
    id:page

    property bool active: status===PageStatus.Active

    //-----------------------------------------
    function rafraichir() {
        var tmp = DesktopService.getList();
        listModel.remplir(tmp);
    }

    //-----------------------------------------
    function toutSauvegarder() {
        for(var i=0 ; i<listModel.count;i++) {
            BackupService.backupPour(listModel.get(i).did);
        }
    }

    //-----------------------------------------
    ListModel {
        id: listModel

        //-----------------------------------------
        function remplir(l) {
            l=l.sort(function(a,b) {
                var ta = a.name.toLowerCase();
                var tb = b.name.toLowerCase();
                if(ta < tb) {return -1;}
                if(ta > tb) {return 1;}
                return 0;
            });

            for(var i in l) {
                if(l[i].type !== "SAILFISH") {
                    continue;
                }

               append({
                  did:l[i].id,
                  name:l[i].name,
                  icon:l[i].icon
              });
            }
        }
    }

    SilicaListView {
        id: fileList
        anchors.fill: parent
        contentWidth: parent.width
        clip: true
        spacing: Theme.paddingLarge
        anchors.bottomMargin: barreinfo.open ? barreinfo.height : 0

        VerticalScrollDecorator { flickable:parent }

        PullDownMenu {
            MenuItem {
                text: vi18n.get("menu.config")
                onClicked: pageStack.push(Qt.resolvedUrl("config.qml"))
            }

            MenuItem {
                text: vi18n.get( enabled ? "menu.sauver" :  (SdcardLocator.present ? "menu.entravail" : "menu.pasdesdcard") )
                enabled: BackupService.entravail===false && SdcardLocator.present
                onClicked: toutSauvegarder()
            }
        }

        header: PageHeader {
            id:titre
            title: vi18n.get("applications.titre")
        }

        model: listModel

        delegate: RowApplicationDelegate{}

        ViewPlaceholder {
            enabled: listModel.count===0
            text: vi18n.get("applications.vide")
            hintText: vi18n.get("applications.vide.sum")
        }
    }

    DockedPanel {
        id:barreinfo
        width: Screen.width
        height: Theme.iconSizeMedium
        dock: Dock.Bottom
        open: BackupService.nbBackup>0

        Label {
            text: vi18n.get("applications.nb").arg(BackupService.nbBackup)
            color: Theme.highlightColor
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Component.onCompleted:  {
        rafraichir();
    }
}
