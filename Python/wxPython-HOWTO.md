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
