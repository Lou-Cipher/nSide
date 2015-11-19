SMPRegisterEditor *smpRegisterEditor = nullptr;

SMPRegisterEditor::SMPRegisterEditor() {
  setTitle("SMP Register Editor");

  layout.setMargin(5);
  regALabel.setText("A:");
  regAValue.setFont(program->monospaceFont);
  regXLabel.setText("X:");
  regXValue.setFont(program->monospaceFont);
  regYLabel.setText("Y:");
  regYValue.setFont(program->monospaceFont);
  regSLabel.setText("S:01");
  regSValue.setFont(program->monospaceFont);
  flagN.setText("N");
  flagV.setText("V");
  flagP.setText("P");
  flagB.setText("B");
  flagH.setText("H");
  flagI.setText("I");
  flagZ.setText("Z");
  flagC.setText("C");
  update.setText("Update");

  loadRegisters();

  layout.append(primaryLayout, {~0, 0}, 5);
    primaryLayout.append(regALabel, {0, 0}, 5);
    primaryLayout.append(regAValue, {0, 0}, 5);
    primaryLayout.append(regXLabel, {0, 0}, 5);
    primaryLayout.append(regXValue, {0, 0}, 5);
    primaryLayout.append(regYLabel, {0, 0}, 5);
    primaryLayout.append(regYValue, {0, 0}, 5);
    primaryLayout.append(regSLabel, {0, 0}, 5);
    primaryLayout.append(regSValue, {0, 0});
  layout.append(secondaryLayout, {~0, 0}, 5);
    secondaryLayout.append(flagN, {0, 0}, 5);
    secondaryLayout.append(flagV, {0, 0}, 5);
    secondaryLayout.append(flagP, {0, 0}, 5);
    secondaryLayout.append(flagB, {0, 0}, 5);
    secondaryLayout.append(flagH, {0, 0}, 5);
    secondaryLayout.append(flagI, {0, 0}, 5);
    secondaryLayout.append(flagZ, {0, 0}, 5);
    secondaryLayout.append(flagC, {0, 0});
  layout.append(tertiaryLayout, {~0, 0});
    tertiaryLayout.append(spacer, {~0, 0});
    tertiaryLayout.append(update, {80, 0});
  append(layout);

  update.onActivate = [&] {
    saveRegisters();
    smpDebugger->updateDisassembly();
    setVisible(false);
  };

  setGeometry({{128, 128}, layout.minimumGeometry().size()});
  windowManager->append(this, "SMPRegisterEditor");
}

void SMPRegisterEditor::loadRegisters() {
  regAValue.setText(hex<2>(SuperFamicom::smp.regs.a));
  regXValue.setText(hex<2>(SuperFamicom::smp.regs.x));
  regYValue.setText(hex<2>(SuperFamicom::smp.regs.y));
  regSValue.setText(hex<2>(SuperFamicom::smp.regs.s));
  flagN.setChecked(SuperFamicom::smp.regs.p.n);
  flagV.setChecked(SuperFamicom::smp.regs.p.v);
  flagP.setChecked(SuperFamicom::smp.regs.p.p);
  flagB.setChecked(SuperFamicom::smp.regs.p.b);
  flagH.setChecked(SuperFamicom::smp.regs.p.h);
  flagI.setChecked(SuperFamicom::smp.regs.p.i);
  flagZ.setChecked(SuperFamicom::smp.regs.p.z);
  flagC.setChecked(SuperFamicom::smp.regs.p.c);
}

void SMPRegisterEditor::saveRegisters() {
  SuperFamicom::smp.regs.a = hex(regAValue.text());
  SuperFamicom::smp.regs.x = hex(regXValue.text());
  SuperFamicom::smp.regs.y = hex(regYValue.text());
  SuperFamicom::smp.regs.s = hex(regSValue.text());
  SuperFamicom::smp.regs.p.n = flagN.checked();
  SuperFamicom::smp.regs.p.v = flagV.checked();
  SuperFamicom::smp.regs.p.p = flagP.checked();
  SuperFamicom::smp.regs.p.b = flagB.checked();
  SuperFamicom::smp.regs.p.h = flagH.checked();
  SuperFamicom::smp.regs.p.i = flagI.checked();
  SuperFamicom::smp.regs.p.z = flagZ.checked();
  SuperFamicom::smp.regs.p.c = flagC.checked();
}
