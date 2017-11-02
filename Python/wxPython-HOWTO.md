<!-- TOC -->

- [Basic](#basic)
    - [Basic wxPython App Structure: wx.App, wx.Frame and wx.Panel](#basic-wxpython-app-structure-wxapp-wxframe-and-wxpanel)
    - [Major Classes](#major-classes)
    - [Events](#events)
    - [Windows Contents Update](#windows-contents-update)
        - [Background](#background)
- [Controls](#controls)
    - [List Controls](#list-controls)
        - [ListBox](#listbox)
        - [ListCtrl](#listctrl)
        - [ObjectListView](#objectlistview)
- [Window Splitter](#window-splitter)

<!-- /TOC -->

# Basic

## Basic wxPython App Structure: wx.App, wx.Frame and wx.Panel

NOTES:
* `wx.App` and `wx.Frame` do NOT have parent-child relationship
* `wx.Frame` and `wx.Panel` have parent-child relationship

```python
class Adb_MainFrame(wx.Frame):
    def __init__(self, parent):
        wx.Frame.__init__(self, parent=parent)

        # Panel and its sizer
        self.panel = wx.Panel(self)
        sizer = wx.BoxSizer(wx.VERTICAL)
        self.outputWnd = wx.TextCtrl(parent = self.panel, id = -1, style = wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_AUTO_URL)
        self.inputLine = wx.TextCtrl(parent = self.panel, id = -1, style=wx.TE_PROCESS_ENTER)

        sizer.Add(self.outputWnd, 1, wx.ALL | wx.EXPAND )
        sizer.Add(self.inputLine, 0, wx.ALL | wx.EXPAND )
        self.panel.SetSizer(sizer)

        # frame's accelerator table
        self.accel_tbl = wx.AcceleratorTable([
                            (wx.ACCEL_NORMAL, wx.WXK_LEFT, sckDecFrameIdx),
                            (wx.ACCEL_NORMAL, wx.WXK_RIGHT, sckIncFrameIdx),
                         ])
        self.SetAcceleratorTable(self.accel_tbl)

        self.Layout()
        self.Show()

class Adb_App(wx.App):
    def OnInit(self):
        self.mainFrame = Adb_MainFrame( parent=None,title="", size=(900, 700))
        self.mainFrame.Show()
        return True
if __name__ == "__main__":
    app = Adb_App()
    app.MainLoop()
```

## Major Classes

[Nice diagrams showing all the major classes](https://www.tutorialspoint.com/wxpython/wxpython_major_classes.htm)

## Events

```python
def _EventMaker():
    evt = wx.NewEventType()
    return (evt, wx.PyEventBinder(evt))
```

## Windows Contents Update

```python
self.Bind(wx.EVT_PAINT, self.OnPaint)
self.Bind(wx.EVTSIZE, self.OnSize)
self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
```

### Background
To avoid flickering
```python
# RESIZE will trigger full repaint => should not be done IF resize can be handled
# once instead of OnPaint even if the window size has not been changed
wx.Panel.__init__(self, parent, -1, style=wx.FULL_REPAINT_ON_RESIZE)
```

# Controls

[wxPython Widget Inspection Tool](https://wiki.wxpython.org/Widget%20Inspection%20Tool): show all the widgets


## List Controls

* ListBox
* ListCtrl
* ObjectListView: a 3rd party control derived from ListCtrl but much easier to use. (also a C# wrapper for C#'s ListView)

### ListBox
```python
# single selection & sorted
lb = wx.ListBox(self, size=(-1,-1), style=wx.LB_SINGLE | wx.LB_SORT)
lb.SetFont( fontSmall ); lb.SetBackgroundColour( bgColor ); lb.SetForegroundColour( fgColor )

for vidName in listVideoName: lb.Append( vidName )

# selection changed
self.Bind(wx.EVT_LISTBOX, self.OnListBox, lb)

def OnListBox(event):
    event.GetEventObject().GetStringSelection()

```

### ListCtrl

[ListCtrl's events](https://wxpython.org/Phoenix/docs/html/wx.ListCtrl.html#events-events-emitted-by-this-class)

```python
#A ListCtrl widget in report view is constructed in the following example.
lc = wx.ListCtrl(panel, -1, style = wx.LC_REPORT)
lc.SetFont( fontSmall )
lc.SetBackgroundColour( bgColor )
lc.SetForegroundColour( fgColor )

#Header columns are created by InsertColumn() method which takes the 
# column number, caption, style and width parameters.
lc.InsertColumn(0, 'name', width = 100) 
lc.InsertColumn(1, 'runs', wx.LIST_FORMAT_RIGHT, 100) 

players = [('Tendulkar', '15000'), 
           ('Dravid', '14000', '1')] 

for p in players: 
   # start a new row
   idxCurRow = lc.InsertStringItem(sys.maxint, p[0]) # sys.maxint: get row # after last row
   # fill the remaining columns
   lc.SetStringItem(idxCurRow, 1, p[1]) 

sel = GetSelectedItemsIndices(lc) # support multi-selection
for i in sel:
    f = lc.GetItemText( i )  # column 0

def GetSelectedItemsIndices(lc):
    listSel = []
    current = -1         # start at -1 to get the first selected item
    while True:
        #current = lc.GetNextItem( current, wx.LIST_NEXT_ALL, wx.LIST_STATE_SELECTED )
        current = lc.GetNextSelected(current)
        if current == -1: break
        listSel.append(current)            
    return listSel

# 
lc.Bind(wx.EVT_LIST_ITEM_ACTIVATED, OnActivation) # enter or double-click current item
lc.Bind(wx.EVT_LIST_ITEM_SELECTED, OnSelected)

def OnSelected(event):
    event.GetText() # column 0 of the selected item
    
def OnActivation(event):
    event.GetText() # column 0 of the selected item
    #print event.GetItem().GetText() # same as above    

#lc.Select(idxItem, on=1) # select/deselect an item
```

### ObjectListView

Derived from `wx.ListCtrl` [Well documented](http://objectlistview.sourceforge.net/python/gettingStarted.html)

```python
listBooks = [{"title":"Core Python Programming", "price":29.99},
             {"title":"Python Programming", "price": 10.99}
            ]

olv = ObjectListView(self, wx.ID_ANY, style=wx.LC_REPORT|wx.SUNKEN_BORDER)            
# Allow the cell values to be edited when double-clicked
self.dataOlv.cellEditMode = ObjectListView.CELLEDIT_SINGLECLICK
            
olv.SetColumns([ ColumnDefn("Title", "left", 220, "title" # velueGetter: can be a class method
                           ),
                 ColumnDefn("Author", "left", 200, "price", 
                  stringConverter="$%.2f" # can be a class method AS WELL
                 )])
olv.SetObjects(listBooks)

```

# Window Splitter

```python
class AppFrame(wx.Frame):
    def __init__(self, parent, title, videoFile):
        wx.Frame.__init__(self, parent=parent, title=title) #, size=size)
        self.mainPanel = AppMainPanel(self)
        
class AppMainPanel( wx.Panel ):
    """
    Parent-children Relationship:
      Panel -> splitter -> top & bottom panes
      
    Programming Relationship:
      Panel -> sizer -> splitter -> top & bottom panes
    """
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent)

        self.horizontalSplitter = wx.SplitterWindow(self)
        self.TopPane = TopPane(self.horizontalSplitter)
        self.btmPane = BottomePane(self.horizontalSplitter)
        self.horizontalSplitter.SplitHorizontally(self.TopPane, self.btmPane, 0)
        self.horizontalSplitter.SetMinimumPaneSize(60)

        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.horizontalSplitter, 1, wx.EXPAND)
        self.SetSizer(self.sizer)

class TopPane (wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent)
        self.Bind(wx.EVT_PAINT, self.OnPaint)

    def OnPaint(self, evt):
        dc = wx.BufferedPaintDC(self)
        dc.SetBrush(wx.Brush(wx.BLACK, wx.SOLID))
        tgtWidth, tgtHeight = self.GetSize()
        dc.DrawRectangle(0, 0, tgtWidth, tgtHeight)

class BottomePane (wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent)
        self.Bind(wx.EVT_PAINT, self.OnPaint)

    def OnPaint(self, evt):
        dc = wx.BufferedPaintDC(self)
        dc.SetBrush(wx.Brush(wx.RED, wx.SOLID))
        tgtWidth, tgtHeight = self.GetSize()
        dc.DrawRectangle(0, 0, tgtWidth, tgtHeight)

```
