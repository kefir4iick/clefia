import SwiftUI
import AppKit


struct EncryptView: View {
    @State private var inputFileURL: URL?
    @State private var keyFileURL: URL?
    @State private var outputFileURL: URL?
    @State private var selectedMode: String = "ECB"
    @State private var selectedKeySize: Int = 256

    var body: some View {
        ZStack {
            Image("back")
                .resizable()
                .scaledToFill()
                .ignoresSafeArea()
            

            Color.black.opacity(0.4)
                .ignoresSafeArea()


            VStack {
                Spacer()

                VStack(spacing: 30) {
                    Text("Encryption")
                        .font(.largeTitle)
                        .fontWeight(.bold)
                        .foregroundColor(.white)

                    Text("Drag & drop files to encrypt")
                        .foregroundColor(.white.opacity(0.8))


                    FileDropZone(
                        title: "File to Encrypt",
                        fileURL: $inputFileURL,
                        allowedExtensions: ["txt", "pdf", "jpg", "png", "doc", "docx"] //покашо так
                    )


                    FileDropZone(
                        title: "Encryption Key",
                        fileURL: $keyFileURL,
                        allowedExtensions: ["key", "pem", "txt"]
                    )


                    VStack(spacing: 15) {
                        Text("Encryption Settings")
                            .font(.headline)
                            .foregroundColor(.white)

                        Picker("Mode", selection: $selectedMode) {
                            Text("ECB").tag("ECB")
                            Text("CBC").tag("CBC")
                            Text("CTR").tag("CTR")
                        }
                        .pickerStyle(.segmented)

                        Picker("Key Length", selection: $selectedKeySize) {
                            Text("128-bit").tag(128)
                            Text("192-bit").tag(192)
                            Text("256-bit").tag(256)
                        }
                        .pickerStyle(.segmented)
                    }
                    .padding()
                    .background(Color.blue.opacity(0.2))
                    .cornerRadius(12)
                    .overlay(
                        RoundedRectangle(cornerRadius: 12)
                            .stroke(Color.blue.opacity(0.5), lineWidth: 2)
                    )


                    VStack {
                        Text("Output File")
                            .font(.headline)
                            .foregroundColor(.white)

                        if let outputURL = outputFileURL {
                            Text(outputURL.lastPathComponent)
                                .foregroundColor(.green)
                                .padding()
                                .background(Color.black.opacity(0.5))
                                .cornerRadius(8)
                        } else {
                            Text("Will be generated here")
                                .foregroundColor(.gray)
                                .padding()
                                .background(Color.black.opacity(0.3))
                                .cornerRadius(8)
                        }
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.blue.opacity(0.2))
                    .cornerRadius(12)
                    .overlay(
                        RoundedRectangle(cornerRadius: 12)
                            .stroke(Color.blue.opacity(0.5), lineWidth: 2)
                    )


                    Button(action: {
                        encryptFile()
                    }) {
                        Text("Encrypt File")
                            .font(.title3)
                            .fontWeight(.semibold)
                            .frame(maxWidth: .infinity)
                            .padding(.vertical, 14)
                            .foregroundColor(.black)
                    }
                    .background(
                        inputFileURL == nil || keyFileURL == nil
                        ? Color.green.opacity(0.4)
                        : Color(red: 0.0, green: 0.8, blue: 0.3)
                    )
                    .foregroundColor(.black)
                    .cornerRadius(10)
                    .disabled(inputFileURL == nil || keyFileURL == nil)
                    .opacity(inputFileURL == nil || keyFileURL == nil ? 0.6 : 1.0)
                }
                .padding(40)
                .frame(maxWidth: 500)
                .background(.ultraThinMaterial)
                .cornerRadius(20)

                Spacer()
            }
        }
        .navigationTitle("Encrypt")
    }

    
    private func encryptFile() {
        guard let inputURL = inputFileURL,
              let keyURL = keyFileURL else { return }

        let inputAccess = inputURL.startAccessingSecurityScopedResource()
        let keyAccess = keyURL.startAccessingSecurityScopedResource()
            
        defer {
            if inputAccess { inputURL.stopAccessingSecurityScopedResource() }
            if keyAccess { keyURL.stopAccessingSecurityScopedResource() }
        }
        let savePanel = NSSavePanel()
        savePanel.title = "save encrypted file"
        savePanel.nameFieldStringValue = "encrypted_" + inputURL.lastPathComponent
        savePanel.canCreateDirectories = true

        savePanel.begin { response in
            if response == .OK, let destinationURL = savePanel.url {
                
                DispatchQueue.global(qos: .userInitiated).async {
                    do {
                        let inputData = try Data(contentsOf: inputURL)
                        let keyData = try Data(contentsOf: keyURL)
                        
                        let expectedKeyLen = self.selectedKeySize / 8
                        guard keyData.count >= expectedKeyLen else {
                            print("error: key is too small")
                            return
                        }

                        var ctx = cl_ctx()
                        let keyLenEnum = self.keySizeToEnum(self.selectedKeySize)
                        let keyBytes = [UInt8](keyData)
                        clefia_init(&ctx, keyBytes, keyLenEnum)
                        
                        let inputBytes = [UInt8](inputData)

                        let bufferSize = ((inputBytes.count + 15) / 16) * 16
                        var outputBytes = [UInt8](repeating: 0, count: bufferSize)
                        
                        var iv = [UInt8](repeating: 0, count: 16)
                        let mode = self.modeToEnum(self.selectedMode)
                        
                        if mode == ECB {
                            encrypt_file(&ctx, mode, nil, inputBytes, &outputBytes, inputBytes.count)
                        } else {
                            encrypt_file(&ctx, mode, iv, inputBytes, &outputBytes, inputBytes.count)
                        }
                        
                        try Data(outputBytes).write(to: destinationURL)
                        
                        
                        DispatchQueue.main.async {
                            self.outputFileURL = destinationURL
                            print("saved in: \(destinationURL.path)")
                        }
                        
                    } catch {
                        print("encrypt error: \(error)")
                    }
                }
            }
        }
    }

    private func modeToEnum(_ mode: String) -> cl_mode {
        switch mode {
        case "ECB": return ECB
        case "CBC": return CBC
        case "CTR": return CTR
        default: return ECB
        }
    }

    private func keySizeToEnum(_ size: Int) -> cl_keylen {
        switch size {
        case 128: return KEY_128
        case 192: return KEY_192
        default: return KEY_256
        }
    }
    
}
