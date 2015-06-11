Public Class BatteryController
    Public Class Parameters
        Public Property BatteryDischargeOn As Single = Single.NaN
        Public Property BatteryDischargeOff As Single = Single.NaN
        Public Property BatteryChargeOn As Single = Single.NaN
        Public Property BatteryChargeOff As Single = Single.NaN
    End Class
    Public Class State
        Public Property IsCharging As Boolean
        Public Property IsDischarging As Boolean
        Public Property VoltageIn As Single = Single.NaN
        Public Property VoltageOut As Single = Single.NaN
        Public Property VoltageBattery As Single = Single.NaN
        Public Property CurrentOut As Single = Single.NaN
    End Class
    Private _ss As SimplSerialBus
    Private _logger As Logger

    Public Sub New(ss As SimplSerialBus)
        _ss = ss
    End Sub

    Public Function GetInfo() As DeviceInfo
        Return _ss.RequestDeviceInfo(0)
    End Function

    Public Sub SetBatterySwitches(charging As Boolean, discharging As Boolean)
        Dim chg = If(charging, 1, 0)
        Dim dschg = If(discharging, 1, 0)
        Dim result = _ss.Request(New SSRequest(0, 71, {chg, dschg, dschg, dschg}))
        If result.ResponseState <> ResponseState.ok And result.ResponseState <> 128 + 71 Then Throw New Exception("SetBatterySwitches: " + result.ToString)
    End Sub

    Public Sub SetLED(ledIndex As Integer, color As Color)
        Dim result = _ss.Request(New SSRequest(0, 70, {ledIndex, color.R, color.G, color.B}))
        If result.ResponseState <> ResponseState.ok And result.ResponseState <> 128 + 70 Then Throw New Exception("SetLED: " + result.ToString)
    End Sub

    Public Function GetButtons() As Boolean()
        Dim result = _ss.Request(New SSRequest(0, 20, {}))
        If result.ResponseState <> ResponseState.ok And result.ResponseState <> 128 + 70 Then Throw New Exception("GetButtons: " + result.ToString)
        Dim buttons As Boolean() = {result.Data(0) > 0, result.Data(1) > 0}
        Return buttons
    End Function

    Public Function GetParameters() As Parameters
        Dim result = _ss.Request(New SSRequest(0, 11, {}))
        If result.ResponseState <> ResponseState.ok And result.ResponseState <> 128 + 11 Then Throw New Exception("GetParameters: " + result.ToString)
        Dim params As New Parameters
        params.BatteryDischargeOff = (CInt(result.Data(1)) << 8 + CInt(result.Data(2))) / 1000.0
        params.BatteryDischargeOn = (CInt(result.Data(4)) << 8 + CInt(result.Data(5))) / 1000.0
        params.BatteryChargeOff = (CInt(result.Data(7)) << 8 + CInt(result.Data(8))) / 1000.0
        params.BatteryChargeOn = (CInt(result.Data(10)) << 8 + CInt(result.Data(11))) / 1000.0
        Return params
    End Function

    Public Function GetState() As State
        Dim result = _ss.Request(New SSRequest(0, 10, {}))
        If result.ResponseState <> ResponseState.ok And result.ResponseState <> 128 + 11 Then Throw New Exception("GetParameters: " + result.ToString)
        Dim state As New State
        state.IsDischarging = result.Data(0) > 0
        state.IsCharging = result.Data(1) > 0
        state.VoltageIn = (CInt(result.Data(6)) << 8 + CInt(result.Data(7))) / 1000.0
        state.VoltageBattery = (CInt(result.Data(9)) << 8 + CInt(result.Data(10))) / 1000.0
        state.CurrentOut = (CInt(result.Data(15)) << 8 + CInt(result.Data(16))) / 1000.0
        Return state
    End Function

End Class
