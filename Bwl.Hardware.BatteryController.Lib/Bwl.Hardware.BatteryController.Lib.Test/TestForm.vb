Public Class TestForm
    Inherits FormAppBase
    Private _ss As New SimplSerialBus(_storage)
    Private _batt As New BatteryController(_ss)
    Private Sub TestForm_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        TryThis(Sub()
                    Dim info = _batt.GetInfo
                    _logger.AddMessage(info.DeviceDate.ToString)
                    _logger.AddMessage(info.DeviceGuid.ToString)
                    _logger.AddMessage(info.DeviceName)
                End Sub)
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        TryThis(Sub()
                    Dim info = _batt.GetParameters
                    _logger.AddMessage("BatteryChargeOn: " + info.BatteryChargeOn.ToString("0.0"))
                    _logger.AddMessage("BatteryChargeOff: " + info.BatteryChargeOff.ToString("0.0"))
                    _logger.AddMessage("BatteryDischargeOn: " + info.BatteryDischargeOn.ToString("0.0"))
                    _logger.AddMessage("BatteryDischargeOff: " + info.BatteryDischargeOff.ToString("0.0"))
                End Sub)

    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        TryThis(Sub()
                    Dim info = _batt.GetState
                    _logger.AddMessage("VoltageIn: " + info.VoltageIn.ToString("0.00"))
                    _logger.AddMessage("VoltageBattery: " + info.VoltageBattery.ToString("0.00"))
                    _logger.AddMessage("VoltageInt0: " + info.Voltageint0.ToString("0.00"))
                    _logger.AddMessage("CurrentOut: " + info.CurrentOut.ToString("0.00"))
                    _logger.AddMessage("IsCharging: " + info.IsCharging.ToString())
                    _logger.AddMessage("IsDischarging: " + info.IsDischarging.ToString())
                End Sub)
    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        TryThis(Sub()
                    Dim info = _batt.GetButtons
                    _logger.AddMessage("GetButtons1: " + info(0).ToString())
                    _logger.AddMessage("GetButtons2: " + info(1).ToString())
                End Sub)
    End Sub

    Private Sub Button5_Click(sender As Object, e As EventArgs) Handles Button5.Click
        TryThis(Sub()
                    _ss.Connect()
                End Sub)
    End Sub

    Public Delegate Sub TryThisDelegate()
    Public Sub TryThis(d As TryThisDelegate)
        Try
            d()
        Catch ex As Exception
            _logger.AddError(ex.Message)
        End Try
    End Sub
End Class