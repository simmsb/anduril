{
  description = "Description for the project";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    mission-control.url = "github:Platonic-Systems/mission-control";
    flake-root.url = "github:srid/flake-root";
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      imports = [
        inputs.mission-control.flakeModule
        inputs.flake-root.flakeModule
      ];
      systems = [ "x86_64-linux" "aarch64-darwin" "x86_64-darwin" ];
      perSystem = { config, self', inputs', pkgs, system, ... }:
        let
          avrlibc = pkgs.pkgsCross.avr.libcCross;
          avr_incflags = [
            "-isystem ${avrlibc}/avr/include"
            "-B${avrlibc}/avr/lib/avr5"
            "-L${avrlibc}/avr/lib/avr5"
            "-B${avrlibc}/avr/lib/avr35"
            "-L${avrlibc}/avr/lib/avr35"
            "-B${avrlibc}/avr/lib/avr51"
            "-L${avrlibc}/avr/lib/avr51"
          ];
          dfp = pkgs.fetchzip {
            url = "http://packs.download.atmel.com/Atmel.ATtiny_DFP.2.0.368.atpack";
            stripRoot = false;
            extension = "zip";
            sha256 = "sha256-T+HZFUZ8QrCFybIkbUGfoITNgJ5vUcb+t4SlvPstPOE=";
          };

        in
        {
          devShells.default = pkgs.mkShell {
            nativeBuildInputs = with pkgs; [
              pkgsCross.avr.libcCross
              pkgsCross.avr.buildPackages.binutils
              pkgsCross.avr.buildPackages.bear
              pkgsCross.avr.buildPackages.gcc9
              avrlibc
              avrdude
            ];
            inputsFrom = [ config.mission-control.devShell config.flake-root.devShell ];
            CFLAGS = "";
            AVR_CFLAGS = avr_incflags;
            AVR_ASFLAGS = avr_incflags;
            ATTINY_DFP = dfp;
          };
          mission-control = {
            wrapperName = "run";
            scripts = {
              # build = {
              #   description = "convert files from .avdl to .avsc";
              #   exec = ''
              #     avro-tools idl2schemata "$FLAKE_ROOT/Hello.avdl" .
              #   '';
              #   category = "Development";
              # };
            };
          };
        };
    };
}

